package hosei.negishi.pdrtam.view;

import hosei.negishi.pdrtam.app.NativeAccesser;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView.Renderer;

public class GLRenderNative implements Renderer {

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		NativeAccesser.getInstance().onSurfaceCreated();
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		NativeAccesser.getInstance().onSurfaceChanged(width, height);
	}

	@Override
	public void onDrawFrame(GL10 gl) {
		NativeAccesser.getInstance().onDrawFrame();
	}

}
