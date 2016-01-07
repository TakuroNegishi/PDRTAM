package hosei.negishi.pdrtam;

import android.os.Environment;

public class Global {
	public static final int IMAGE_WIDTH = 640 * 1;
	public static final int IMAGE_HEIGHT = 480 * 1;
	public static final double PREVIEW_RATIO = 1;
	public static final int PREVIEW_WIDTH = (int) (IMAGE_WIDTH * PREVIEW_RATIO);
	public static final int PREVIEW_HEIGHT = (int) (IMAGE_HEIGHT * PREVIEW_RATIO);
	
	public static final String APP_DATA_DIR_PATH = Environment.getExternalStorageDirectory() + "/negishi.deadreckoning/";
	
	public static final Object sharedResource = new Object();
}
