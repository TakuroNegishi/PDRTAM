package hosei.negishi.pdrtam;

import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.Utils;
import org.opencv.core.CvType;
import org.opencv.core.Mat;

import android.content.Context;
import android.graphics.Bitmap;
import android.util.Log;

public class SLAMEngine {
	private CvCameraManager cameraManager;
	private ATAMNative atam;
	private ATAMThread atamThread;
	private ATAMView atamView;;

	public SLAMEngine(MainActivity activity) {
		cameraManager = new CvCameraManager(activity, (CameraBridgeViewBase) activity.findViewById(R.id.customizableCameraView1));
		atamView = (ATAMView) activity.findViewById(R.id.atamView);
		atam = new ATAMNative();
	}
	
	public void changeState() {
		atam.changeState();
	}
	
	public void reset() {
		atam.setReset();
	}
	
	public void stop() {
		atam.setStop();
	}
	
	public void saveImage() {
		cameraManager.saveImageMat();
	}
	
	public float[] getPoints3D() {
		float[] pointAry = new float[atam.getPointLength()];
		atam.getPointAry(pointAry.length, pointAry);
		return pointAry;
	}
	
	public void onResume() {
		Log.e("", "aa");
		cameraManager.onResume();	// カメラ開始
		if (atamThread == null) {
			atamThread = new ATAMThread();
		}
		atamThread.start();			// ATAM開始
		Log.e("", "bb");
	}
	
	public void onPause() {
		stop();
		atamThread.setFinished(true);
		try {
			atamThread.join();
		} catch (InterruptedException e) {
			Log.e("SLAMEngine.onPause()", e.getMessage());
		}
		reset();
		cameraManager.onPause();
	}
	
	/**
	 * 
	 */
	class ATAMThread extends Thread {
		private Mat copyRgba;
		private boolean isFinished;
		private Bitmap bitmap;
		
		public ATAMThread() {
			isFinished = false;
			copyRgba = new Mat(Global.PREVIEW_HEIGHT, Global.PREVIEW_WIDTH, CvType.CV_8UC4);
			bitmap = Bitmap.createBitmap(copyRgba.width(), copyRgba.height(), Bitmap.Config.ARGB_8888);
		}
		
		@Override
		public void run() {
			while(!isFinished) {
				// カメラプレビューのthreadと同期
				synchronized (Global.sharedResource) {
					if (cameraManager.mRgba == null) continue;
					cameraManager.mRgba.copyTo(copyRgba);
				}
				atam.mainProc(copyRgba);
				// ここでSurfaceViewに描画
				Utils.matToBitmap(copyRgba, bitmap);
				atamView.doDraw(bitmap);
			}
			// Thread終了時に解法
			copyRgba.release();
		}
		
		public void setFinished(boolean flag) {
			isFinished = flag;
		}
	}
}
