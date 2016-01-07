package hosei.negishi.pdrtam;

import java.util.LinkedList;

public class FPSCounter {
	private final int WINDOW_SIZE = 5;
	
	private long preTime;
	private LinkedList<Integer> fpsHistory;

	public FPSCounter() {
		preTime = 0;
		fpsHistory = new LinkedList<Integer>();
	}
	
	public int calcFPS() {
		long nowTime = System.currentTimeMillis();
		int nowFPS = (int) (1000 / (nowTime - preTime));
		preTime = nowTime;
		// WINDOW_SIZE分の移動平均
		fpsHistory.addLast(nowFPS);
		if (fpsHistory.size() > WINDOW_SIZE)
			fpsHistory.removeFirst();
		int averageFPS = 0;
		for (Integer fps : fpsHistory) {
			averageFPS += fps;
		}
		averageFPS /= fpsHistory.size();
		return averageFPS;
		// logTextView.setText(averageFPS + "FPS");
	}
}
