package hosei.negishi.pdrtam.app;

import org.opencv.core.Mat;

/**
 * Naitiveアクセス用クラス
 * */
public class NativeAccesser {
	static {
//		System.loadLibrary("gnustl_shared");
//		System.loadLibrary("stlport_shared");
		System.loadLibrary("ATAMNative");
	}
	
	private static final NativeAccesser instance = new NativeAccesser();
	
	public static NativeAccesser getInstance() {
        return instance;
    }
	
	private NativeAccesser() {
		init();
	}
	
	public void init() {
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
	
	public float[] getPointAry() {
		float[] pointAry = new float[getPointLength()];
		getPointAryNative(pointAry.length, pointAry);
		return pointAry;
	}
	
	public int getPointLength() {
		return getPointLengthNative();
	}
	
	public void destroy() {
		// TODO
	}
	
	// ATAM
	public native void initNative();
	public native void mainProcNative(long matAddrRgba);
	public native void changeStateNative();
	public native void setStopNative();
	public native void setResetNative();
	public native void getPointAryNative(int num, float[] pointAry);
	public native int getPointLengthNative();
}
