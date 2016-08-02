package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.content.Context;
import android.util.Log;
import java.util.Collections;
import java.util.Set;
import java.util.HashSet;
import java.util.HashMap;

final class BluetoothDeviceWrapper {
    private static final String TAG = "BluetoothDeviceWrapper";

    private BluetoothDevice mDevice;
    private BluetoothGattWrapper mGatt;
    private BluetoothGattCallbackWrapper mCallback;
    private HashMap<Integer, BluetoothGattServiceWrapper> mServices;
    private HashMap<Integer, BluetoothGattCharacteristicWrapper> mCharacteristics;
    private HashMap<Integer, BluetoothGattDescriptorWrapper> mDescriptors;
    private boolean mConnected;

    public BluetoothDeviceWrapper(BluetoothDevice device) {
        mDevice = device;
        mServices = new HashMap<Integer, BluetoothGattServiceWrapper>();
        mCharacteristics = new HashMap<Integer, BluetoothGattCharacteristicWrapper>();
        mDescriptors = new HashMap<Integer, BluetoothGattDescriptorWrapper>();
        mConnected = false;
    }

    public static BluetoothDeviceWrapper create(BluetoothDevice device) {
        return new BluetoothDeviceWrapper(device);
    }

    public String getAddress() {
        return mDevice.getAddress();
    }

    public String getName() {
        return mDevice.getName();
    }

    public boolean isConnected() {
        return mConnected;
    }

    public void setConnected(boolean connected) {
        mConnected = connected;
    }

    public BluetoothGattWrapper connectGatt(Context context, boolean autoConnect) {
        if (mGatt != null) {
            if (mConnected) {
                mGatt.discoverServices();
            } else {
                mGatt.connect();
            }
            return mGatt;
        }
        mGatt = BluetoothGattWrapper.create(
            mDevice.connectGatt(context, /*autoConnect*/ false,
                BluetoothGattCallbackWrapper.create(
                    this)/*, BluetoothDevice.TRANSPORT_LE*/), this);
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
            BluetoothGattServiceWrapper.create(service, this, service.hashCode()));
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
            BluetoothGattCharacteristicWrapper.create(characteristic, this, characteristic.hashCode()));
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
            BluetoothGattDescriptorWrapper.create(descriptor, this, descriptor.hashCode()));
    }

    public int getDescriptorsSize() {
        return mServices.values().size();
    }
}
