package hosei.negishi.pdrtam;

import org.opencv.core.Mat;

/**
 * NaitiveATAMアクセス用クラス
 * */
public class ATAMNative {
	static {
//		System.loadLibrary("gnustl_shared");
//		System.loadLibrary("stlport_shared");
		System.loadLibrary("ATAMNative");
	}
	
	public ATAMNative() {
		initNative();
	}
	
	public void mainProc(Mat rgba) {
		mainProcNative(rgba.getNativeObjAddr());
	}
	
	public void changeState() {
		changeStateNative();
	}
	
	public void setStop() {
		setStopNative();
	}
	
	public void setReset() {
		setResetNative();
	}
	
	public void getPointAry(int num, float[] pointAry) {
		getPointAryNative(num, pointAry);
	}
	
	public int getPointLength() {
		return getPointLengthNative();
	}
	
	public void destroy() {
		
	}
	
	public native void initNative();
	public native void mainProcNative(long matAddrRgba);
	public native void changeStateNative();
	public native void setStopNative();
	public native void setResetNative();
	public native void getPointAryNative(int num, float[] pointAry);
	public native int getPointLengthNative();
	
}
