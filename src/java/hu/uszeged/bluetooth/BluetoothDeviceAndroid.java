package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.util.Log;
import java.util.Collections;
import java.util.Set;

final class BluetoothDeviceAndroid {
    private static final String TAG = "BluetoothDeviceAndroid";

    private BluetoothDevice mDevice;
    private BluetoothGattAndroid mGatt;
    private BluetoothGattCallbackAndroid mCallback;

    public BluetoothDeviceAndroid(BluetoothDevice device) {
        Log.i(TAG, "ctor");
        mDevice = device;
    }

    public static BluetoothDeviceAndroid create(BluetoothDevice device) {
        Log.i(TAG, "create");
        return new BluetoothDeviceAndroid(device);
    }

    public String getAddress() {
        Log.i(TAG, "getAddress");
        return mDevice.getAddress();
    }

    public String getName() {
        Log.i(TAG, "getName");
        return mDevice.getName();
    }

    public BluetoothGattAndroid connectGatt(Context context, boolean autoConnect) {
        Log.i(TAG, "connectGatt");
        if (mGatt != null) {
            mGatt.connect();
            return mGatt;
        }
        return BluetoothGattAndroid.create(
            mDevice.connectGatt(context, autoConnect,
                BluetoothGattCallbackAndroid.create(
                    this)));
    }
}
