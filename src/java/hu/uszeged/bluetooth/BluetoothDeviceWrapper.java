package hu.uszeged.bluetooth;

import android.app.Application;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.os.ParcelUuid;
import java.util.UUID;
import java.util.Set;
import java.util.HashSet;
import java.util.List;

final class BluetoothDeviceWrapper {
    private BluetoothDevice mDevice;
    private BluetoothGattWrapper mGatt;
    private BluetoothGattCallbackWrapper mCallback;
    private int mRssi;
    private ScanRecord mScanRecord;
    private Set<String> mUuids;
    private boolean mConnected;

    public BluetoothDeviceWrapper(BluetoothDevice device, int rssi, byte[] scanRecord) {
        mDevice = device;
        mRssi = rssi;
        mScanRecord = ScanRecord.parseFromBytes(scanRecord);
        mUuids = new HashSet<String>();
        mConnected = false;
        initUuids();
    }

    public static BluetoothDeviceWrapper create(BluetoothDevice device, int rssi, byte[] scanRecord) {
        return new BluetoothDeviceWrapper(device, rssi, scanRecord);
    }

    public String getAddress() {
        return mDevice.getAddress();
    }

    public String getName() {
        String name = mDevice.getName();
        if (name == null)
            name = mScanRecord.getDeviceName();
        return name;
    }

    public Set<String> getUuids() {
        return mUuids;
    }

    public int getUuidsSize() {
        return mUuids.size();
    }

    public int getRssi() {
        return mRssi;
    }

    public int getTxPower() {
        return mScanRecord.getTxPowerLevel();
    }

    public int[] getManufacturerDataKeys() {
        return mScanRecord.getManufacturerDataKeys();
    }

    public int getManufacturerDataKeysSize() {
        return mScanRecord.getManufacturerDataKeysSize();
    }

    public int[] getManufacturerDataValues(int key) {
        return mScanRecord.getManufacturerDataValues(key);
    }

    public int getManufacturerDataValuesSize(int key) {
        return mScanRecord.getManufacturerDataValuesSize(key);
    }

    public Set<String> getServiceDataKeys() {
        return mScanRecord.getServiceDataKeys();
    }

    public int getServiceDataKeysSize() {
        return mScanRecord.getServiceDataKeysSize();
    }

    public int[] getServiceDataValues(String key) {
        return mScanRecord.getServiceDataValues(key);
    }

    public int getServiceDataValuesSize(String key) {
        return mScanRecord.getServiceDataValuesSize(key);
    }

    public boolean isConnected() {
        return mConnected;
    }

    public void setConnected(boolean connected) {
        if (!connected)
            mGatt = null;
        mConnected = connected;
    }

    public static Application getApplicationUsingReflection() throws Exception {
        return (Application) Class.forName("android.app.ActivityThread")
            .getMethod("currentApplication").invoke(null, (Object[]) null);
    }

    public boolean connectGatt() {
        if (mGatt != null) {
            if (mConnected) {
                mGatt.discoverServices();
            } else {
                mGatt.connect();
            }
        } else {
            try {
                Context ctx = getApplicationUsingReflection();
                mGatt = BluetoothGattWrapper.create(
                    mDevice.connectGatt(ctx, /*autoConnect*/ false,
                        BluetoothGattCallbackWrapper.create(
                            this)/*, BluetoothDevice.TRANSPORT_LE*/), this);
            } catch (final Exception e) {
                setConnected(false);
                return false;
            }
        }
        return true;
    }

    public boolean disconnect() {
        mGatt.disconnect();
        return true;
    }

    public BluetoothGattWrapper getGatt() {
        return mGatt;
    }

    private void initUuids() {
        List<ParcelUuid> uuids = mScanRecord.getServiceUuids();
        if (uuids == null)
            return;
        for (ParcelUuid uuid : mScanRecord.getServiceUuids()) {
            mUuids.add(uuid.getUuid().toString());
        }
    }
}
