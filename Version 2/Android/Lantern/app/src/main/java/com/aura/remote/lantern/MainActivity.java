package com.aura.remote.lantern;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;

public class MainActivity extends AppCompatActivity {
    private Button btn;
    private EditText hours;

    public class ToneGenerator {
        Thread t;
        int sr = 44100;
        boolean isRunning = true;

        public void startTone(final Double frequency) {
            isRunning = true;
            t = new Thread() {
                public void run() {
                    // set process priority
                    setPriority(Thread.MAX_PRIORITY);
                    int buffsize = AudioTrack.getMinBufferSize(sr,
                            AudioFormat.CHANNEL_OUT_MONO,
                            AudioFormat.ENCODING_PCM_16BIT);
                    // create an audiotrack object
                    AudioTrack audioTrack = new AudioTrack(
                            AudioManager.STREAM_MUSIC, sr,
                            AudioFormat.CHANNEL_OUT_MONO,
                            AudioFormat.ENCODING_PCM_16BIT, buffsize,
                            AudioTrack.MODE_STREAM);

                    short samples[] = new short[buffsize];
                    int amp = 10000;
                    double twopi = 8. * Math.atan(1.);
                    double fr = frequency;
                    double ph = 0.0;
                    // start audio
                    audioTrack.play();

                    while (isRunning) {

                        for (int i = 0; i < buffsize; i++) {
                            short s = (short) (amp * Math.sin(ph));
                            if (s > 0.0) {
                                samples[i] = 32767;
                            }
                            ;

                            if (s < 0.0) {
                                samples[i] = -32767;
                            }
                            ph += twopi * fr / sr;
                        }
                        audioTrack.write(samples, 0, buffsize);
                    }
                    audioTrack.stop();
                    audioTrack.release();
                }
            };
            t.start();
        }

        public void stopTone() {
            isRunning = false;
            try {
                t.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            t = null;
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }
}
