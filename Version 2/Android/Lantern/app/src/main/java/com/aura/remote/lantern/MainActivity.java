package com.aura.remote.lantern;

import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;
import java.util.LinkedList;
import java.util.concurrent.TimeUnit;


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
        btn = findViewById(R.id.b1);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                hours = findViewById(R.id.hours);
                String hour = hours.getText().toString();

                if (hour.matches("")) {
                    Toast.makeText(getApplicationContext(), "Enter Hours!!!!!", Toast.LENGTH_SHORT).show();
                    return;
                }

                try {
                    String Time = java.text.DateFormat.getTimeInstance().format(new Date());
                    String date_L = java.text.DateFormat.getDateInstance().format(new Date());

                    Toast.makeText(getApplicationContext(), "Current Time is " + Time, Toast.LENGTH_SHORT).show();
                    Toast.makeText(getApplicationContext(), "Current Date is " + date_L, Toast.LENGTH_SHORT).show();
                    //TimeUnit.MILLISECONDS.sleep(1000);
                    GregorianCalendar receievedTime = new GregorianCalendar();
                    receievedTime.add(Calendar.HOUR_OF_DAY, Integer.parseInt(hour));
                    Toast.makeText(getApplicationContext(), "Receieved Time is " + receievedTime.getTime(), Toast.LENGTH_SHORT).show();
                    String recHr = receievedTime.getTime().toString().substring(11, 13);
                    Toast.makeText(getApplicationContext(), "Receieved Time...... : " + recHr, Toast.LENGTH_SHORT).show();

                    //long diff = Long.parseLong(recHr.toString());
                    //Toast.makeText(getApplicationContext(), "Diff in MS" + diff, Toast.LENGTH_SHORT).show();
                    //long diffHours = diff / (60 * 60 * 1000);
                    //Toast.makeText(getApplicationContext(), "Diff in HRS" + diffHours, Toast.LENGTH_SHORT).show();
                    //TimeUnit.MILLISECONDS.sleep(1000);

                    String binHour = Integer.toBinaryString(Integer.parseInt(recHr));
                    //Toast.makeText(getApplicationContext(), "Bin Hour String!!!!!"+binHour, Toast.LENGTH_SHORT).show();
                    int sbinHour = Integer.parseInt(binHour);
                    if (sbinHour > 16) {
                        Toast.makeText(getApplicationContext(), "Enter Time Difference less than 16!!!!!", Toast.LENGTH_SHORT).show();
                        return;
                    }
                    if (sbinHour == 0) {
                        Toast.makeText(getApplicationContext(), "Enter Time After Current Time in hrs!!!!!", Toast.LENGTH_SHORT).show();
                        return;
                    }
                    Toast.makeText(getApplicationContext(), "Bin Hour Integers!!!!!" + sbinHour, Toast.LENGTH_SHORT).show();
                    LinkedList<Integer> stack = new LinkedList<Integer>();
                    while (sbinHour > 0) {
                        stack.push(sbinHour % 10);
                        sbinHour = sbinHour / 10;
                    }
                    ToneGenerator firstBit = new ToneGenerator();
                    ToneGenerator secondBit = new ToneGenerator();
                    ToneGenerator thirdBit = new ToneGenerator();
                    ToneGenerator fourthBit = new ToneGenerator();
                    int bit = stack.pop();
                    if (bit == 0) {
                        firstBit.startTone(Double.parseDouble("1100"));
                        TimeUnit.MILLISECONDS.sleep(300);
                        firstBit.stopTone();
                    } else {
                        firstBit.startTone(Double.parseDouble("2100"));
                        TimeUnit.MILLISECONDS.sleep(300);
                        firstBit.stopTone();
                    }
                    bit = stack.pop();
                    if (bit == 0) {
                        secondBit.startTone(Double.parseDouble("3100"));
                        TimeUnit.MILLISECONDS.sleep(300);
                        secondBit.stopTone();
                    } else {
                        secondBit.startTone(Double.parseDouble("4100"));
                        TimeUnit.MILLISECONDS.sleep(300);
                        secondBit.stopTone();
                    }
                    bit = stack.pop();
                    if (bit == 0) {
                        thirdBit.startTone(Double.parseDouble("5100"));
                        TimeUnit.MILLISECONDS.sleep(300);
                        thirdBit.stopTone();
                    } else {
                        thirdBit.startTone(Double.parseDouble("6100"));
                        TimeUnit.MILLISECONDS.sleep(300);
                        thirdBit.stopTone();
                    }
                    bit = stack.pop();
                    if (bit == 0) {
                        fourthBit.startTone(Double.parseDouble("7100"));
                        TimeUnit.MILLISECONDS.sleep(300);
                        fourthBit.stopTone();
                    } else {
                        fourthBit.startTone(Double.parseDouble("8100"));
                        TimeUnit.MILLISECONDS.sleep(300);
                        fourthBit.stopTone();
                    }

                    //Toast.makeText(getApplicationContext(), "Data Send ...!", Toast.LENGTH_SHORT).show();


                } catch (Exception e) {
                    Toast.makeText(getApplicationContext(), "Error  ...!"+e.getLocalizedMessage(), Toast.LENGTH_SHORT).show();
                }
            }
        });


    }
}
