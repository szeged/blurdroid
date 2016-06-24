package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothGattService;
import android.util.Log;

final class BluetoothGattServiceAndroid {
    private static final String TAG = "BluetoothGattServiceAndroid";

    private BluetoothGattService mService;

    public BluetoothGattServiceAndroid(BluetoothGattService service) {
        Log.i(TAG, "ctor");
        mService = service;
    }

    public static BluetoothGattServiceAndroid create(BluetoothGattService service) {
        Log.i(TAG, "create");
        return new BluetoothGattServiceAndroid(service);
    }

    public String getUuid() {
        Log.i(TAG, "getUuid");
        return mService.getUuid().toString();
    }
}
