package hosei.negishi.pdrtam.app;

import hosei.negishi.pdrtam.R;
import hosei.negishi.pdrtam.model.FileChooser;
import hosei.negishi.pdrtam.utils.Config;
import hosei.negishi.pdrtam.utils.FileManager;
import hosei.negishi.pdrtam.view.CustomGLSurfaceView;

import java.io.File;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class MainActivity extends Activity implements OnClickListener,
		DialogInterface.OnClickListener {
	private AppMode appMode;
	private SLAMEngine slam;

	private CustomGLSurfaceView glSurfaceView;
	private FileChooser fileChooser;

	// ライブラリ初期化完了後に呼ばれるコールバック (onManagerConnected)
	// public abstract class BaseLoaderCallback implements
	// LoaderCallbackInterface
	private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
		@Override
		public void onManagerConnected(int status) {
			switch (status) {
			// 読み込みが成功したらカメラプレビューを開始
			case LoaderCallbackInterface.SUCCESS:
				slam.onResume();
				break;
			default:
				super.onManagerConnected(status);
				break;
			}
		}
	};

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		// getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		// // バックライト常にON
		// getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN); //
		// ステータスバー非表示
		// requestWindowFeature(Window.FEATURE_NO_TITLE); // タイトルバー非表示

		appMode = AppMode.TAM;
		setContentView(R.layout.activity_main);
		initTAMMode();
		initButtons();
	}

	private void initTAMMode() {
		slam = new SLAMEngine(this);
		glSurfaceView = (CustomGLSurfaceView)findViewById(R.id.customGLSurfaceView);
	}

	public void initButtons() {
		Button btn = (Button) findViewById(R.id.change_btn);
		btn.setOnClickListener(this);
		btn = (Button) findViewById(R.id.view3d_btn);
		btn.setOnClickListener(this);
		btn = (Button) findViewById(R.id.reset_btn);
		btn.setOnClickListener(this);
		btn = (Button) findViewById(R.id.takePic_btn);
		btn.setOnClickListener(this);
	}

	public void init3DViewer() {
		glSurfaceView.onPause();
//		glSurfaceView = null;
		glSurfaceView = new CustomGLSurfaceView(this);
	}

	public void start3DViewer() {
		appMode = AppMode.OBSERVATION;
		setContentView(glSurfaceView);
		glSurfaceView.onResume();
	}

//	public void stop3DViewer() {
//		appMode = AppMode.CAMERA_PREVIEW;
//		setContentView(R.layout.activity_main);
//		glSurfaceView.onPause();
//	}
//
	@Override
	protected void onResume() {
		super.onResume();
		glSurfaceView.onResume();
		if (appMode == AppMode.TAM) {
			// 非同期でライブラリの読み込み/初期化を行う
			// static boolean initAsync(String Version, Context AppContext,
			// LoaderCallbackInterface Callback)
			OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_3_0_0, this,
					mLoaderCallback);
			// OpenCVライブラリロード終了次第slam.onResume()でATAM開始
		}
	}

	@Override
	public void onPause() {
		super.onPause();
		glSurfaceView.onPause();
		if (appMode == AppMode.TAM)
			slam.onPause();
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		glSurfaceView.dispose();
		glSurfaceView = null;
//		slam.onPause();
	}

	@Override
	public void onClick(View v) {
		if (!(v instanceof Button))
			return;
		switch (v.getId()) {
		case R.id.takePic_btn:
			slam.saveImage();
			break;
		case R.id.change_btn:
			slam.changeState();
			// saveImageMat(mRgba);
			break;
		case R.id.reset_btn:
			slam.reset();
			break;
		case R.id.view3d_btn:
			fileChooser = new FileChooser();
			fileChooser.showFiles(new File(Config.APP_DATA_DIR_PATH), this);
			break;
		}
	}

	@Override
	public void onClick(DialogInterface dialog, int which) {
		if (fileChooser.isDirectory(which)) {
			fileChooser.showFiles(which, this); // 選択したディレクトリ内のファイル表示
		} else {
			// 選択したファイルから点群情報を読み取る
			String filePath = fileChooser.getFilePath(which);
			fileChooser = null;
			if (filePath.equals(""))
				return;
			float[] vertex = FileManager.readFile(filePath);
			init3DViewer();
			glSurfaceView.setVertex(vertex);
			slam.onPause();
			start3DViewer();
		}
	}

//	@Override
//	public boolean onTouchEvent(MotionEvent event) {
//		if (appMode != AppMode.MAP_VIEW)
//			return super.onTouchEvent(event);
//		glSurfaceView.onTouch(event);
//		return super.onTouchEvent(event);
//	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK) { // Androidの戻るボタン押下→終了ダイアログ
			showFinishDialog();

//			switch (appMode) {
//			case TAM:
//				showFinishDialog();
//				break;
//			}
			return true;
		}
		return false;
	}

	/** アプリケーション終了確認ダイアログ表示 */
	private void showFinishDialog() {
		new AlertDialog.Builder(this)
				.setTitle("アプリケーションの終了")
				.setMessage("アプリケーションを終了しますか？")
				.setPositiveButton("Yes",
						new DialogInterface.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog,
									int which) {
								MainActivity.this.finish();
							}
						})
				.setNegativeButton("No", new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
					}
				}).show();
	}

}
