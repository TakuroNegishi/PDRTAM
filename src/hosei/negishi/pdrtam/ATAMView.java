package hosei.negishi.pdrtam;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.hardware.Camera.PreviewCallback;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

public class ATAMView extends SurfaceView implements SurfaceHolder.Callback {

	private SurfaceHolder holder;
	
	public ATAMView(Context context) {
		super(context);
		holder = getHolder();
	}
	
	public ATAMView(Context context, AttributeSet attrs) {
		super(context, attrs);
		holder = getHolder();
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		this.holder = holder;
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		
	}
	
	public void doDraw(Bitmap bitmap) {
		Canvas canvas = holder.lockCanvas();
//		canvas.setBitmap(bitmap);
//		canvas.drawColor(Color.BLACK);
		canvas.drawBitmap(bitmap, 0, 0, null);
		holder.unlockCanvasAndPost(canvas);
	}
	
	@Override
	public void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
//		super.onMeasure(widthMeasureSpec, heightMeasureSpec);
		setMeasuredDimension(Global.PREVIEW_WIDTH, Global.PREVIEW_HEIGHT);
	}
}
