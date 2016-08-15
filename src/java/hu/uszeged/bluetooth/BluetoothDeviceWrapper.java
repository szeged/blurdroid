package hu.uszeged.bluetooth;

import android.app.Application;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.content.Context;
import android.os.ParcelUuid;
import android.util.Log;
import java.io.StringWriter;
import java.io.PrintWriter;
import java.util.UUID;
import java.util.Set;
import java.util.HashSet;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;

final class BluetoothDeviceWrapper {
    private BluetoothDevice mDevice;
    private BluetoothGattWrapper mGatt;
    private BluetoothGattCallbackWrapper mCallback;
    private HashMap<Integer, BluetoothGattServiceWrapper> mServices;
    private HashMap<Integer, BluetoothGattCharacteristicWrapper> mCharacteristics;
    private HashMap<Integer, BluetoothGattDescriptorWrapper> mDescriptors;
    private int mRssi;
    private ScanRecord mScanRecord;
    private Set<String> mUuids;
    private boolean mConnected;

    public BluetoothDeviceWrapper(BluetoothDevice device, int rssi, byte[] scanRecord) {
        mDevice = device;
        mRssi = rssi;
        mScanRecord = ScanRecord.parseFromBytes(scanRecord);
        mServices = new HashMap<Integer, BluetoothGattServiceWrapper>();
        mCharacteristics = new HashMap<Integer, BluetoothGattCharacteristicWrapper>();
        mDescriptors = new HashMap<Integer, BluetoothGattDescriptorWrapper>();
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

    public boolean isConnected() {
        return mConnected;
    }

    public void setConnected(boolean connected) {
        mConnected = connected;
    }

    public static Application getApplicationUsingReflection() throws Exception {
        return (Application) Class.forName("android.app.ActivityThread")
            .getMethod("currentApplication").invoke(null, (Object[]) null);
    }

    public BluetoothGattWrapper connectGatt() {
        if (mGatt != null) {
            if (mConnected) {
                mGatt.discoverServices();
            } else {
                mGatt.connect();
            }
            return mGatt;
        }
        try {
            Context ctx = getApplicationUsingReflection();
            mGatt = BluetoothGattWrapper.create(
                mDevice.connectGatt(ctx, /*autoConnect*/ false,
                    BluetoothGattCallbackWrapper.create(
                        this)/*, BluetoothDevice.TRANSPORT_LE*/), this);
        } catch (final Exception e) {
            mGatt = null;
        }
        return mGatt;
    }

    public void disconnect() {
        mGatt.disconnect();
    }

    public BluetoothGattWrapper getGatt() {
        return mGatt;
    }

    public Set<BluetoothGattServiceWrapper> getServices() {
        return new HashSet<BluetoothGattServiceWrapper>(mServices.values());
    }

    public BluetoothGattServiceWrapper getService(int id) {
        return mServices.get(id);
    }

    public void addService(BluetoothGattService service) {
        mServices.put(service.hashCode(),
            BluetoothGattServiceWrapper.create(service, this));
    }

    public int getServicesSize() {
        return mServices.values().size();
    }

    public Set<BluetoothGattCharacteristicWrapper> getCharacteristics() {
        return new HashSet<BluetoothGattCharacteristicWrapper>(mCharacteristics.values());
    }

    public BluetoothGattCharacteristicWrapper getCharacteristic(int id) {
        return mCharacteristics.get(id);
    }

    public void addCharacteristic(BluetoothGattCharacteristic characteristic) {
        mCharacteristics.put(characteristic.hashCode(),
            BluetoothGattCharacteristicWrapper.create(characteristic, this));
    }

    public int getCharacteristicsSize() {
        return mCharacteristics.values().size();
    }

    public Set<BluetoothGattDescriptorWrapper> getDescriptors() {
        return new HashSet<BluetoothGattDescriptorWrapper>(mDescriptors.values());
    }

    public BluetoothGattDescriptorWrapper getDescriptor(int id) {
        return mDescriptors.get(id);
    }

    public void addDescriptor(BluetoothGattDescriptor descriptor) {
        mDescriptors.put(descriptor.hashCode(),
            BluetoothGattDescriptorWrapper.create(descriptor, this));
    }

    public int getDescriptorsSize() {
        return mServices.values().size();
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
