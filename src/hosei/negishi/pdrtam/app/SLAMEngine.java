package hosei.negishi.pdrtam.app;

import hosei.negishi.pdrtam.R;

import org.opencv.android.CameraBridgeViewBase;

public class SLAMEngine {
	private CvCameraManager cameraManager;
	private ATAMNative atam;
	
	public SLAMEngine(MainActivity activity) {
		atam = new ATAMNative();
		cameraManager = new CvCameraManager(activity, atam, (CameraBridgeViewBase) activity.findViewById(R.id.customizableCameraView1));
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
		cameraManager.onResume();	// カメラ開始
	}
	
	public void onPause() {
		stop();
		reset();
		cameraManager.onPause();
	}
}
