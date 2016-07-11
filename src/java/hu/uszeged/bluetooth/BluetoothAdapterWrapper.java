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
        Log.i(TAG, "###################<");
        Log.i(TAG, "ctor");
        Log.i(TAG, "###################>");
        mAdapter = adapter;
        mDevices = new HashMap<String, BluetoothDeviceWrapper>();
        //mDevices = new ArrayList<BluetoothDeviceWrapper>();
    }

    public static BluetoothAdapterWrapper getAdapter() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getAdapter");
        Log.i(TAG, "###################>");
        return new BluetoothAdapterWrapper(
            BluetoothAdapter.getDefaultAdapter());
    }

    public boolean isPresent() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "isPresent");
        Log.i(TAG, "###################>");
        boolean present = mAdapter != null;
        if (!present) {
            Log.e(TAG, "!!!NO ADAPTER!!!");
        }
        return present;
    }

    public String getAddress() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getAddress");
        Log.i(TAG, "###################>");
        if (isPresent()) {
            Log.i(TAG, "address: "+mAdapter.getAddress());
            return mAdapter.getAddress();
        } else {
            return "";
        }
    }

    public String getName() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getName");
        Log.i(TAG, "###################>");
        if (isPresent()) {
            Log.i(TAG, "name: "+mAdapter.getName());
            return mAdapter.getName();
        } else {
            return "";
        }
    }

    public Set<BluetoothDeviceWrapper> getDevices() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getDevices");
        Log.i(TAG, "###################>");
        return new HashSet<BluetoothDeviceWrapper>(mDevices.values());
    }

    public Set<BluetoothDeviceWrapper> getBondedDevices() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getBondedDevices");
        Log.i(TAG, "###################>");
        for (BluetoothDevice device : mAdapter.getBondedDevices()) {
            addDevice(device);
        }
        return getDevices();
    }

    public void addDevice(BluetoothDevice device) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "addDevice");
        Log.i(TAG, "###################>");
        if (!mDevices.containsKey(device.getAddress())) {
            mDevices.put(device.getAddress(), BluetoothDeviceWrapper.create(device));
        }
    }

    public int getDevicesSize() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getDevicesSize "+mDevices.size());
        Log.i(TAG, "###################>");
        return mDevices.size();
    }

    public BluetoothDeviceWrapper getRemoteDevice(String address) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getRemoteDevice");
        Log.i(TAG, "###################>");
        if (isPresent()) {
            return mDevices.get(address);
        } else {
            return null;
        }
    }

    public boolean isEnabled() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "isEnabled");
        Log.i(TAG, "###################>");
        if (isPresent()) {
            Log.i(TAG, "isEnabled "+mAdapter.isEnabled());
            return mAdapter.isEnabled();
        } else {
            return false;
        }
    }

    public boolean enable() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "enable");
        Log.i(TAG, "###################>");
        if (isPresent()) {
            Log.i(TAG, "enable "+mAdapter.enable());
            return mAdapter.enable();
        } else {
            return false;
        }
    }

    public boolean disable() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "disable");
        Log.i(TAG, "###################>");
        if (isPresent()) {
            Log.i(TAG, "disable "+mAdapter.disable());
            return mAdapter.disable();
        } else {
            return false;
        }
    }

    public void startLeScan() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "startLeScan");
        Log.i(TAG, "###################>");
        mAdapter.startLeScan(mLeScanCallback);
    }

    public void stopLeScan() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "stopLeScan");
        Log.i(TAG, "###################>");
        mAdapter.stopLeScan(mLeScanCallback);
    }

    private BluetoothAdapter.LeScanCallback mLeScanCallback =
            new BluetoothAdapter.LeScanCallback() {
        @Override
        public void onLeScan(final BluetoothDevice device, int rssi, byte[] scanRecord) {
            //runOnUiThread(new Runnable() {
            //    @Override
            //    public void run() {
            Log.i(TAG, "###################<");
            Log.i(TAG, "onLeScan "+device.getAddress());
            Log.i(TAG, "###################>");
                    addDevice(device);
            //    }
            //});
        }
    };
}
