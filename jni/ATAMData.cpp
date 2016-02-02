/*!
@file		ATAMData.cpp
@brief		functions in ATAMData.h
*/
#include <jni.h>
#include <android/log.h>
#include <opencv2/calib3d.hpp>
#include "ATAMData.h"

#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, __FILE__, __VA_ARGS__))

sATAMParams PARAMS;		//!< parameters in ATAM

/*!
@brief		constructor
*/
sPose::sPose(void)
{
	rvec = cv::Mat::eye(cvSize(1, 3), CV_64F);
	tvec = cv::Mat::eye(cvSize(1, 3), CV_64F);
}

/*!
@brief		copy constructor
@param[in]	r	right side
*/
sPose::sPose(const sPose& r)
{
	r.rvec.copyTo(rvec);
	r.tvec.copyTo(tvec);
}

/*!
@brief		= operator
@param[in]	r	right side
*/
sPose& sPose::operator=(const sPose& r)
{
	r.rvec.copyTo(rvec);
	r.tvec.copyTo(tvec);
	return *this;
}

/*!
@brief		get rvec and tvec in M
@param[out]	M	4 x 4 transformation matrix
*/
void sPose::getM(cv::Mat &M) const
{
	M = cv::Mat::eye(cvSize(4, 4), CV_64F);

	// 3x1 rotation vector to 3x3 rotation matrix
	cv::Mat r;
	cv::Rodrigues(rvec, r);

	r.copyTo(M(cv::Rect(0, 0, 3, 3)));
	tvec.copyTo(M(cv::Rect(3, 0, 1, 3)));
}

/*!
@brief		get  rotation matrix
@param[out]	R	4 x 4 transformation matrix
*/
void sPose::getR(cv::Mat &R) const
{
	cv::Rodrigues(rvec, R);
}

/*!
@brief		set rvec and tvec from M
@param[in]	m	4 x 4 transformation matrix
*/
void sPose::setFromM(const cv::Mat &M)
{
	cv::Mat r;

	M(cv::Rect(0, 0, 3, 3)).copyTo(r);
	cv::Rodrigues(r, rvec);

	M(cv::Rect(3, 0, 1, 3)).copyTo(tvec);
}

/*!
@brief	for debugging
*/
void sPose::print(void) const
{
//	__android_log_print(ANDROID_LOG_ERROR, "NativeCode", "%s(%d)", __FILE__, __VA_ARGS__);
//	LOGE(rvec.t());
//	LOGE(tvec.t());
//	std::cout << rvec.t() << std::endl;
//	std::cout << tvec.t() << std::endl;
}

/*!
@brief	constructor
*/
sTrack::sTrack(void)
{
	ptID = NOID;
}

/*!
@brief	clear data
*/
void sKeyframe::clear(void)
{
	vkptID.clear();
	vkpt.clear();
	vdesc.release();

	vptID.clear();
	vpt.clear();

	ID = NOID;
}

/*!
@brief	clear data
*/
void sBAData::clear(void)
{
	vkeyframe.clear();
	vkeyframeID.clear();
	vpt3d.clear();
	vvisibleID.clear();
}

/*!
@brief	constructor
*/
CMapData::CMapData(void)
{
	Clear();
}

/*!
@brief	clear data
*/
void CMapData::Clear(void)
{
	mMapMutex.lock();

	mvPt.clear();
	mvKf.clear();
	mAdded = false;

	mMapMutex.unlock();
}

/*!
@brief			copy data to BA thread
@param[out]		data	BA data
*/
bool CMapData::CopytoBA(sBAData &data)
{
	if (mAdded){

		mMapMutex.lock();

		if (mvPt.size() == 0){
			return false;
		}

		int prevsize = int(data.vpt3d.size());
		data.vpt3d.resize(mvPt.size());
		for (int i = prevsize, iend = int(mvPt.size()); i < iend; ++i){
			data.vpt3d[i] = mvPt[i];
		}

		int size = int(mvKf.size()) > PARAMS.BAKEYFRAMES ? PARAMS.BAKEYFRAMES : int(mvKf.size());

		data.vkeyframe.clear();
		data.vkeyframeID.clear();

		data.vkeyframe.resize(size);
		data.vkeyframeID.resize(size);

		for (int i = 0; i < size; ++i){
			int id = int(mvKf.size()) - size + i;
			data.vkeyframeID[i] = id;
			data.vkeyframe[i].pose = mvKf[id].pose;
			data.vkeyframe[i].vpt = mvKf[id].vpt;
			data.vkeyframe[i].vptID = mvKf[id].vptID;
		}

		mAdded = false;

		mMapMutex.unlock();

		return true;
	}
	else{
		return false;
	}
}

/*!
@brief		copy data from BA thread
@param[in]	data	BA data
*/
void CMapData::CopyfromBA(const sBAData &data)
{
	if (mvPt.size() != 0){

		mMapMutex.lock();
		for (int i = 0, iend = int(data.vvisibleID.size()); i < iend; ++i){
			int id = data.vvisibleID[i];
			mvPt[id] = data.vpt3d[id];
		}

		for (int i = 0, iend = int(data.vkeyframeID.size()); i < iend; ++i){
			int id = data.vkeyframeID[i];
			mvKf[id].pose = data.vkeyframe[i].pose;
		}
		mMapMutex.unlock();
	}
}

/*!
@brief	add keyframe
@param[in]	kf		keyframe
@param[in]	vdesc	descriptors
*/
void CMapData::AddKeyframe(const sKeyframe &kf, const cv::Mat &vdesc)
{
	mMapMutex.lock();

	mvKf.push_back(kf);

	sKeyframe &lkf = mvKf.back();

	lkf.ID = int(mvKf.size()) - 1;		// set keyframe ID
	vdesc.copyTo(lkf.vdesc);

	mAdded = true;

	LOGE("%d points in keyframes %d at %s\n", lkf.vkpt.size(), lkf.ID, __FUNCTION__);

	mMapMutex.unlock();
}

/*!
@brief		return last keyframe
@retval		sKeyframe
*/
sKeyframe& CMapData::GetLastKeyframe(void)
{
	return mvKf.back();
}

/*!
@brief		update last keyframe
@param[in]	vpt3d	mapped 3d points
@param[in]	vkpt	corresponding keypoints
@param[in]	vdesc	vkpt's descriptors
@param[out]	vid		vkpt's keypoint ID
*/
void CMapData::UpdateLastKeyframe(
	const std::vector<cv::Point3f> &vpt3d,
	const std::vector<cv::KeyPoint> &vkpt,
	const cv::Mat &vdesc,
	std::vector<int> &vid
	)
{
	mMapMutex.lock();

	// get last keyframe
	sKeyframe &lkf = mvKf.back();

	// get point ID
	int pointID = int(mvPt.size());

	// set point ID to points in the keyframe
	for (int i = 0, iend = int(vpt3d.size()); i < iend; ++i, ++pointID){
		mvPt.push_back(vpt3d[i]);

		lkf.vkpt.push_back(vkpt[i]);
		lkf.vkptID.push_back(pointID);

		lkf.vpt.push_back(vkpt[i].pt);
		lkf.vptID.push_back(pointID);

		vid.push_back(pointID);
	}

	// compute descriptor
	lkf.vdesc.push_back(vdesc);

	LOGE("Added %d points Total keypoints %d in keyframe %d\n", int(vpt3d.size()), lkf.vkpt.size(), lkf.ID);

	mMapMutex.unlock();
}

/*!
@brief		return point
@param[in]	id		point ID
@retval		point
*/
const cv::Point3f& CMapData::GetPoint(const int id) const
{
	return mvPt[id];
}

std::vector<cv::Point3f> CMapData::GetAllPoint() {
	std::vector<cv::Point3f> tmpPt;

	mMapMutex.lock();
	copy(mvPt.begin(), mvPt.end(), back_inserter(tmpPt));
	mMapMutex.unlock();

	return tmpPt;
}

/*!
@brief		return nearest keyframe from all keyframes
@param[in]	pose	pose
@retval		keyframe
*/
const sKeyframe& CMapData::GetNearestKeyframe(const sPose &pose) const
{
	double dist = cv::norm(mvKf[0].pose.tvec - pose.tvec);
	int ID = 0;

	for (int i = 1, iend = int(mvKf.size()); i < iend; ++i){
		double tmp = cv::norm(mvKf[i].pose.tvec - pose.tvec);
		if (tmp < dist){
			dist = tmp;
			ID = i;
		}
	}

	return mvKf[ID];
}

/*!
@brief			return randomly selected pose for relocalization
@param[out]		pose	pose
*/
void CMapData::GetPoseforRelocalization(sPose &pose) const
{
	int size = PARAMS.RELOCALHIST > int(mvKf.size()) ? int(mvKf.size()) : PARAMS.RELOCALHIST;
	int val = rand() % size;
	int index = int(mvKf.size()) - 1 - val;

	pose = mvKf[index].pose;
}

/*!
@brief			return good pose for relocalization
@param[out]		pose	pose
*/
void CMapData::GetGoodPoseforRelocalization(sPose &pose) const
{
//	LOGE("start GetGoodPoseforRelocalization()");
	int size = PARAMS.RELOCALHIST > int(mvKf.size()) ? int(mvKf.size()) : PARAMS.RELOCALHIST;

	// select keyframe depending on number of keypoints in keyframe
	int maxPts = -1;
	for (int i = int(mvKf.size()) - size; i < int(mvKf.size()); ++i){
		int pts = int(mvKf[i].vkpt.size());

		if (maxPts < pts){
			maxPts = pts;
			pose = mvKf[i].pose;
		}
	}
//	LOGE("end GetGoodPoseforRelocalization()");
}

/*!
@brief			return map size
@retval			map size
*/
int CMapData::GetSize(void)
{
	mMapMutex.lock();
	int size = int(mvPt.size());
	mMapMutex.unlock();

	return size;
}

/*!
@brief		constructor
*/
sATAMParams::sATAMParams()
{
	MAXPTS = 400;		// ORBの最大検出ポイント数
	LEVEL = 2;			// ORBのレベル
	DESCDIST = 50.f;	// matchKeyframe()時の許容対応点距離(DESCDIST以上離れるとout)

	BASEANGLE = 5.0;	//
	BAKEYFRAMES = 4;	//

	// solvePnPの結果と再投影時の点の誤差許容範囲
	PROJERR = 3.f;
	// 最低トラッキングポイント数(オプティカルフローが正しく計算出来た点がMINPTS未満だとrelocal)
	MINPTS = 20; // def
//	MINPTS = 10;
	// オプティカルフローや3次元点→2次元点に写像された点が画像内に存在するかに使う.
	// 画像外枠からPATCHSIZE*2分離れた四角枠内にある点のみ「画像内に存在する」とする
//	PATCHSIZE = 10;
	PATCHSIZE = 17; // def
	// matchKeyframe()のsolvePnPRansac()時の目標inlier率(inliers/all)
	MATCHKEYFRAME = 0.4f;

	GOODINIT = 0.9f; // initialのmakeMap()時の目標inlier率(inliers/all)
	RELOCALHIST = 5;

//	VIDEONAME = strData + "movie.avi";
	USEVIDEO = false;

//	CAMERANAME = strData + "camera.xml";
}

/*!
@brief		load parameters
@param[in]	file name
*/
void sATAMParams::loadParams(const std::string &name)
{
	cv::FileStorage fs;

	if (!fs.open(name, cv::FileStorage::READ)){
		fs.open(name, cv::FileStorage::WRITE);

		// about keypoint
		cv::write(fs, "MAXPTS", MAXPTS);
		cv::write(fs, "LEVEL", LEVEL);
		cv::write(fs, "DESCDIST", DESCDIST);

		// about mapping
		cv::write(fs, "BASEANGLE", BASEANGLE);
		cv::write(fs, "BAKEYFRAMES", BAKEYFRAMES);

		// about tracking
		cv::write(fs, "PROJERR", PROJERR);
		cv::write(fs, "MINPTS", MINPTS);
		cv::write(fs, "PATCHSIZE", PATCHSIZE);
		cv::write(fs, "MATCHKEYFRAME", MATCHKEYFRAME);

		cv::write(fs, "GOODINIT", GOODINIT);
		cv::write(fs, "RELOCALHIST", RELOCALHIST);

		cv::write(fs, "USEVIDEO", int(USEVIDEO));
		cv::write(fs, "VIDEONAME", VIDEONAME);

		cv::write(fs, "CAMERANAME", CAMERANAME);
	}
	else{
		cv::FileNode node(fs.fs, NULL);

		MAXPTS = node["MAXPTS"];
		LEVEL = node["LEVEL"];
		DESCDIST = node["DESCDIST"];

		BASEANGLE = node["BASEANGLE"];
		BAKEYFRAMES = node["BAKEYFRAMES"];

		PROJERR = node["PROJERR"];
		MINPTS = node["MINPTS"];
		PATCHSIZE = node["PATCHSIZE"];
		MATCHKEYFRAME = node["MATCHKEYFRAME"];

		GOODINIT = node["GOODINIT"];
		RELOCALHIST = node["RELOCALHIST"];

		USEVIDEO = int(node["USEVIDEO"]) == 1 ? true : false;
		VIDEONAME = std::string(node["VIDEONAME"]);

		CAMERANAME = std::string(node["CAMERANAME"]);
	}

	BASETAN = tan(BASEANGLE / 180.0*CV_PI);
}

/*!
@brief	constructor
*/
sATAMData::sATAMData(void)
{
	clear();
}

/*!
@brief	clear data
*/
void sATAMData::clear(void)
{
	clearTrack();

	baData.clear();
	map.Clear();
	vKpt.clear();

	transMat = cv::Mat::eye(cvSize(4, 4), CV_64F);

	vposePair.clear();

	havescale = false;
}

/*!
@brief	clear tracking data
*/
void sATAMData::clearTrack(void)
{
	vtrack.clear();
	vprevpt.clear();
}

/*!
@brief		clear track
@param[in]	ID		point ID
*/
void sATAMData::clearTrack(const int ID)
{
	for (std::list<sTrack>::iterator it = vtrack.begin(),
		itend = vtrack.end(); it != itend;){
		if (it->ptID == ID){
			it = vtrack.erase(it);
		}
		else{
			++it;
		}
		if (it == vtrack.end()){
			break;
		}
	}

	vprevpt.clear();

	for (std::list<sTrack>::iterator it = vtrack.begin(),
		itend = vtrack.end(); it != itend; ++it){
		vprevpt.push_back(it->vpt.back());
	}
}

/*!
@brief		set point for tracking (set kpt and ptID beforehand)
@param[in]	in	 track
*/
void sATAMData::addTrack(const sTrack &in)
{
	const cv::Point2f &pt = in.kpt.pt;
	vtrack.push_back(in);
	vtrack.back().vpt.push_back(pt);
	vprevpt.push_back(pt);
}
