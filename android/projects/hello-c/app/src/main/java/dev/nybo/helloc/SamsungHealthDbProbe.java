package dev.nybo.helloc;

import android.content.Context;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;

public final class SamsungHealthDbProbe {
    private static final String TAG = "SamsungHealthDbProbe";
    private static final String[] PATHS = new String[] {
            "",
            "heartrate",
            "heart_rate",
            "heartRate",
            "heart_rate_record",
            "com.samsung.health.heart_rate",
            "steps",
            "summary"
    };
    private static final String AUTHORITY = "com.samsung.android.wear.shealth.healthdataprovider";
    private static final String[] CALL_METHODS = new String[] {
            "heart_rate",
            "heart_rate.today.hourly",
            "stress.today.hourly",
            "exercise.last.workout",
            "workout.this.week",
            "multisport.this.week",
            "action.button.setting",
            "steps.today.hourly",
            "sleep",
            "sleep.today.detail",
            "spo2",
            "oxygen_saturation"
    };

    private SamsungHealthDbProbe() {
    }

    public static void run(Context context) {
        for (String method : CALL_METHODS) {
            try {
                Bundle bundle = context.getContentResolver().call(AUTHORITY, method, "register", null);
                Log.i(TAG, "call " + method + " register -> " + bundleToString(bundle));
                bundle = context.getContentResolver().call(AUTHORITY, method, null, null);
                Log.i(TAG, "call " + method + " -> " + bundleToString(bundle));
            } catch (Throwable t) {
                Log.i(TAG, "call " + method + " -> " + t.getClass().getSimpleName() + ": " + t.getMessage());
            }
        }
        for (String path : PATHS) {
            Uri uri = Uri.parse("content://" + AUTHORITY + "/" + path);
            try (Cursor cursor = context.getContentResolver().query(uri, null, null, null, null)) {
                if (cursor == null) {
                    Log.i(TAG, uri + " -> null cursor");
                    continue;
                }
                StringBuilder columns = new StringBuilder();
                for (String column : cursor.getColumnNames()) {
                    if (columns.length() > 0) columns.append(",");
                    columns.append(column);
                }
                Log.i(TAG, uri + " -> rows=" + cursor.getCount() + " columns=" + columns);
                int shown = 0;
                while (cursor.moveToNext() && shown < 3) {
                    StringBuilder row = new StringBuilder();
                    for (int i = 0; i < cursor.getColumnCount(); i++) {
                        if (i > 0) row.append(" | ");
                        row.append(cursor.getColumnName(i)).append("=").append(cursor.getString(i));
                    }
                    Log.i(TAG, uri + " row " + shown + ": " + row);
                    shown++;
                }
            } catch (Throwable t) {
                Log.i(TAG, uri + " -> " + t.getClass().getSimpleName() + ": " + t.getMessage());
            }
        }
    }

    private static String bundleToString(Bundle bundle) {
        if (bundle == null) {
            return "null";
        }
        StringBuilder text = new StringBuilder("{");
        for (String key : bundle.keySet()) {
            if (text.length() > 1) text.append(", ");
            Object value = bundle.get(key);
            text.append(key).append("=").append(value);
        }
        return text.append("}").toString();
    }
}
