package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import java.util.Set;
import java.util.HashSet;
import java.util.HashMap;

final class BluetoothAdapterWrapper {
    private BluetoothAdapter mAdapter;
    private HashMap<String, BluetoothDeviceWrapper> mDevices;

    public BluetoothAdapterWrapper(BluetoothAdapter adapter) {
        mAdapter = adapter;
        mDevices = new HashMap<String, BluetoothDeviceWrapper>();
    }

    public static BluetoothAdapterWrapper getAdapter() {
        return new BluetoothAdapterWrapper(
            BluetoothAdapter.getDefaultAdapter());
    }

    public String getAddress() {
        return mAdapter.getAddress();
    }

    public String getName() {
        return mAdapter.getName();
    }

    public Set<BluetoothDeviceWrapper> getDevices() {
        return new HashSet<BluetoothDeviceWrapper>(mDevices.values());
    }

    public void addDevice(BluetoothDevice device, int rssi, byte[] scanRecord) {
        if (!mDevices.containsKey(device.getAddress()))
            mDevices.put(device.getAddress(), BluetoothDeviceWrapper.create(device, rssi, scanRecord));
    }

    public int getDevicesSize() {
        return mDevices.size();
    }

    public BluetoothDeviceWrapper getRemoteDevice(String address) {
        return mDevices.get(address);
    }

    public boolean isEnabled() {
        return mAdapter.isEnabled();
    }

    public boolean enable() {
        return mAdapter.enable();
    }

    public boolean disable() {
        return mAdapter.disable();
    }

    public void startLeScan() {
        mAdapter.startLeScan(mLeScanCallback);
    }

    public void stopLeScan() {
        mAdapter.stopLeScan(mLeScanCallback);
    }

    private BluetoothAdapter.LeScanCallback mLeScanCallback =
            new BluetoothAdapter.LeScanCallback() {
        @Override
        public void onLeScan(final BluetoothDevice device, int rssi, byte[] scanRecord) {
            addDevice(device, rssi, scanRecord);
        }
    };
}
