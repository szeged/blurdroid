package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattService;
import android.util.Log;
import java.util.Collections;
import java.util.Set;
import java.util.HashSet;

final class BluetoothGattAndroid {
    private static final String TAG = "BluetoothGattAndroid";

    private BluetoothGatt mGatt;

    public BluetoothGattAndroid(BluetoothGatt gatt) {
        Log.i(TAG, "ctor");
        mGatt = gatt;
    }

    public static BluetoothGattAndroid create(BluetoothGatt gatt) {
        Log.i(TAG, "create");
        return new BluetoothGattAndroid(gatt);
    }

    public boolean connect() {
        Log.i(TAG, "connect");
        return mGatt.connect();
    }

    public boolean discoverServices() {
        Log.i(TAG, "discoverServices");
        boolean res = mGatt.discoverServices();
        try
        {
            java.lang.Thread.sleep(1000);
        }
        catch (Exception e)
        {
            System.out.println(e);
        }
        return res;
    }

    //TODO Set -> List
    public Set<BluetoothGattServiceAndroid> getServices() {
        Log.i(TAG, "getServices");
        Set<BluetoothGattServiceAndroid> result
            = new HashSet<BluetoothGattServiceAndroid>();
        for (BluetoothGattService service : mGatt.getServices()) {
            result.add(BluetoothGattServiceAndroid.create(service));
        }
        return result;
    }
}
