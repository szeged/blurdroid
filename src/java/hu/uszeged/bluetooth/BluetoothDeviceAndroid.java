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
    private HashMap<String, BluetoothGattDescriptorWrapper> mDescriptors;

    public BluetoothDeviceWrapper(BluetoothDevice device) {
        Log.i(TAG, "ctor");
        mDevice = device;
        mServices = new HashMap<Integer, BluetoothGattServiceWrapper>();
        mCharacteristics = new HashMap<Integer, BluetoothGattCharacteristicWrapper>();
        mDescriptors = new HashMap<String, BluetoothGattDescriptorWrapper>();
        Log.i(TAG, "device address: "+ mDevice.getAddress());
    }

    public static BluetoothDeviceWrapper create(BluetoothDevice device) {
        Log.i(TAG, "create");
        return new BluetoothDeviceWrapper(device);
    }

    public String getAddress() {
        Log.i(TAG, "getAddress");
        return mDevice.getAddress();
    }

    public String getName() {
        Log.i(TAG, "getName");
        return mDevice.getName();
    }

    public BluetoothGattWrapper connectGatt(Context context, boolean autoConnect) {
        Log.i(TAG, "connectGatt");
        if (mGatt != null) {
            mGatt.connect();
            return mGatt;
        }
        mGatt = BluetoothGattWrapper.create(
            mDevice.connectGatt(context, autoConnect,
                BluetoothGattCallbackWrapper.create(
                    this)), this);
        return mGatt;
    }

    public BluetoothGattWrapper getGatt() {
        Log.i(TAG, "getGatt");
        return mGatt;
    }

    public Set<BluetoothGattServiceWrapper> getServices() {
        Log.i(TAG, "getServices");
        return new HashSet<BluetoothGattServiceWrapper>(mServices.values());
    }

    public BluetoothGattServiceWrapper getService(int id) {
        Log.i(TAG, "getService");
        return mServices.get(id);
    }

    public void addService(BluetoothGattService service) {
        Log.i(TAG, "addService");
        mServices.put(service.getInstanceId(),
            BluetoothGattServiceWrapper.create(service, this));
    }

    public int getServicesSize() {
        return mServices.values().size();
    }

    public Set<BluetoothGattCharacteristicWrapper> getCharacteristics() {
        Log.i(TAG, "getCharacteristics");
        return new HashSet<BluetoothGattCharacteristicWrapper>(mCharacteristics.values());
    }

    public BluetoothGattCharacteristicWrapper getCharacteristic(int id) {
        Log.i(TAG, "getCharacteristic");
        return mCharacteristics.get(id);
    }

    public void addCharacteristic(BluetoothGattCharacteristic characteristic) {
        Log.i(TAG, "addCharacteristics");
        mCharacteristics.put(characteristic.getInstanceId(),
            BluetoothGattCharacteristicWrapper.create(characteristic, this));
    }

    public int getCharacteristicsSize() {
        return mCharacteristics.values().size();
    }

    public Set<BluetoothGattDescriptorWrapper> getDescriptors() {
        Log.i(TAG, "getDescriptors");
        return new HashSet<BluetoothGattDescriptorWrapper>(mDescriptors.values());
    }

    public BluetoothGattDescriptorWrapper getDescriptor(String uuid) {
        Log.i(TAG, "getDescriptor");
        return mDescriptors.get(uuid);
    }

    public void addDescriptor(BluetoothGattDescriptor descriptor) {
        Log.i(TAG, "addDescriptors");
        mDescriptors.put(descriptor.getUuid().toString(),
            BluetoothGattDescriptorWrapper.create(descriptor, this));
    }

    public int getDescriptorsSize() {
        return mServices.values().size();
    }
}
