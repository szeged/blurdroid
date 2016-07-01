package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.content.Context;
import android.util.Log;
import java.util.Collections;
import java.util.Set;
import java.util.HashSet;
import java.util.HashMap;

final class BluetoothAdapterWrapper {
    private static final String TAG = "BluetoothAdapterWrapper";

    private BluetoothAdapter mAdapter;
    private HashMap<String, BluetoothDeviceWrapper> mDevices;
    //private ArrayList<BluetoothDeviceWrapper> mDevices;

    public BluetoothAdapterWrapper(BluetoothAdapter adapter) {
        Log.i(TAG, "ctor");
        mAdapter = adapter;
        mDevices = new HashMap<String, BluetoothDeviceWrapper>();
        //mDevices = new ArrayList<BluetoothDeviceWrapper>();
    }

    public static BluetoothAdapterWrapper getAdapter() {
        Log.i(TAG, "getAdapter");
        return new BluetoothAdapterWrapper(
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

    public Set<BluetoothDeviceWrapper> getDevices() {
        Log.i(TAG, "getDevices");
        return new HashSet<BluetoothDeviceWrapper>(mDevices.values());
    }

    public Set<BluetoothDeviceWrapper> getBondedDevices() {
        Log.i(TAG, "getBondedDevices");
        for (BluetoothDevice device : mAdapter.getBondedDevices()) {
            addDevice(device);
        }
        return getDevices();
    }

    public void addDevice(BluetoothDevice device) {
        Log.i(TAG, "addDevice");
        if (!mDevices.containsKey(device.getAddress())) {
            mDevices.put(device.getAddress(), BluetoothDeviceWrapper.create(device));
        }
    }

    public int getDevicesSize() {
        Log.i(TAG, "getDevicesSize");
        return mDevices.size();
    }

    public BluetoothDeviceWrapper getRemoteDevice(String address) {
        Log.i(TAG, "getRemoteDevice");
        if (isPresent()) {
            return mDevices.get(address);
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

    public void startLeScan() {
        Log.i(TAG, "startLeScan");
        mAdapter.startLeScan(mLeScanCallback);
    }

    public void stopLeScan() {
        Log.i(TAG, "stopLeScan");
        mAdapter.stopLeScan(mLeScanCallback);
    }

    private BluetoothAdapter.LeScanCallback mLeScanCallback =
            new BluetoothAdapter.LeScanCallback() {
        @Override
        public void onLeScan(final BluetoothDevice device, int rssi, byte[] scanRecord) {
            //runOnUiThread(new Runnable() {
            //    @Override
            //    public void run() {
            Log.i(TAG, "onLeScan");
                    addDevice(device);
            //    }
            //});
        }
    };
}
