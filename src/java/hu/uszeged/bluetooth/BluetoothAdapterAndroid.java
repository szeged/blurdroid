package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.util.Log;
import java.util.Collections;
import java.util.Set;
import java.util.HashSet;

final class BluetoothAdapterAndroid {
    private static final String TAG = "BluetoothAdapterAndroid";

    private BluetoothAdapter mAdapter;
    //private Set<BluetoothDeviceAndroid> mDevices;

    public BluetoothAdapterAndroid(BluetoothAdapter adapter) {
        Log.i(TAG, "ctor");
        mAdapter = adapter;
    }

    public static BluetoothAdapterAndroid getAdapter() {
        Log.i(TAG, "getAdapter");
        return new BluetoothAdapterAndroid(
            BluetoothAdapter.getDefaultAdapter());
    }

    public boolean isPresent() {
        Log.i(TAG, "isPresent");
        boolean present = mAdapter != null;
        if (!present) {
            Log.e(TAG, "!!!NO ADAPTER!!!");
        }
        return present;
    }

    public String getAddress() {
        Log.i(TAG, "getAddress");
        if (isPresent()) {
            return mAdapter.getAddress();
        } else {
            return "";
        }
    }

    public String getName() {
        Log.i(TAG, "getName");
        if (isPresent()) {
            return mAdapter.getName();
        } else {
            return "";
        }
    }

    private Set<BluetoothDevice> getBondedDevices() {
        Log.i(TAG, "getBondedDevices");
        if (isPresent()) {
            return mAdapter.getBondedDevices();
        } else {
            return Collections.emptySet();
        }
    }

    public Set<BluetoothDeviceAndroid> getDevices() {
        Log.i(TAG, "getDevices");
        Set<BluetoothDeviceAndroid> result = new HashSet<BluetoothDeviceAndroid>();
        for (BluetoothDevice device : getBondedDevices()) {
            result.add(BluetoothDeviceAndroid.create(device));
        }
        return result;
    }

    public BluetoothDeviceAndroid getRemoteDevice(String address) {
        Log.i(TAG, "getRemoteDevice");
        if (isPresent()) {
            return BluetoothDeviceAndroid.create(
                mAdapter.getRemoteDevice(address));
        } else {
            return null;
        }
    }

    public boolean isEnabled() {
        Log.i(TAG, "isEnabled");
        if (isPresent()) {
            return mAdapter.isEnabled();
        } else {
            return false;
        }
    }

    public boolean enable() {
        Log.i(TAG, "enable");
        if (isPresent()) {
            return mAdapter.enable();
        } else {
            return false;
        }
    }

    public boolean disable() {
        Log.i(TAG, "disable");
        if (isPresent()) {
            return mAdapter.disable();
        } else {
            return false;
        }
    }
}
