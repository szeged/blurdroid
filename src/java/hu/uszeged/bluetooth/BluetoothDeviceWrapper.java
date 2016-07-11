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
        Log.i(TAG, "###################<");
        Log.i(TAG, "ctor");
        Log.i(TAG, "###################>");
        mDevice = device;
        mServices = new HashMap<Integer, BluetoothGattServiceWrapper>();
        mCharacteristics = new HashMap<Integer, BluetoothGattCharacteristicWrapper>();
        mDescriptors = new HashMap<Integer, BluetoothGattDescriptorWrapper>();
        mConnected = false;
        Log.i(TAG, "###################<");
        Log.i(TAG, "device address: "+ mDevice.getAddress());
        Log.i(TAG, "###################>");
    }

    public static BluetoothDeviceWrapper create(BluetoothDevice device) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "create");
        Log.i(TAG, "###################>");
        return new BluetoothDeviceWrapper(device);
    }

    public String getAddress() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getAddress "+mDevice.getAddress());
        Log.i(TAG, "###################>");
        return mDevice.getAddress();
    }

    public String getName() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getName "+mDevice.getName());
        Log.i(TAG, "###################>");
        return mDevice.getName();
    }

    public boolean isConnected() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "isConnected" + mConnected);
        Log.i(TAG, "###################>");
        return mConnected;
    }

    public void setConnected(boolean connected) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "setConnected " + connected);
        Log.i(TAG, "###################>");
        mConnected = connected;
    }

    public BluetoothGattWrapper connectGatt(Context context, boolean autoConnect) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "connectGatt");
        Log.i(TAG, "###################>");
        if (mGatt != null) {
            if (mConnected) {
                mGatt.discoverServices();
            } else {
                mGatt.connect();
            }
            return mGatt;
        }
        mGatt = BluetoothGattWrapper.create(
            mDevice.connectGatt(context, /*autoConnect*/ true,
                BluetoothGattCallbackWrapper.create(
                    this)/*, BluetoothDevice.TRANSPORT_LE*/), this);
        return mGatt;
    }

    public BluetoothGattWrapper getGatt() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getGatt");
        Log.i(TAG, "###################>");
        return mGatt;
    }

    public Set<BluetoothGattServiceWrapper> getServices() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getServices");
        Log.i(TAG, "###################>");
        return new HashSet<BluetoothGattServiceWrapper>(mServices.values());
    }

    public BluetoothGattServiceWrapper getService(int id) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getService");
        Log.i(TAG, "###################>");
        return mServices.get(id);
    }

    public void addService(BluetoothGattService service) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "addService");
        Log.i(TAG, "###################>");
        mServices.put(service.hashCode(),
            BluetoothGattServiceWrapper.create(service, this));
    }

    public int getServicesSize() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getServicesSize");
        Log.i(TAG, "###################>");
        return mServices.values().size();
    }

    public Set<BluetoothGattCharacteristicWrapper> getCharacteristics() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getCharacteristics");
        Log.i(TAG, "###################>");
        return new HashSet<BluetoothGattCharacteristicWrapper>(mCharacteristics.values());
    }

    public BluetoothGattCharacteristicWrapper getCharacteristic(int id) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getCharacteristic");
        Log.i(TAG, "###################>");
        return mCharacteristics.get(id);
    }

    public void addCharacteristic(BluetoothGattCharacteristic characteristic) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "addCharacteristics");
        Log.i(TAG, "###################>");
        mCharacteristics.put(characteristic.hashCode(),
            BluetoothGattCharacteristicWrapper.create(characteristic, this));
    }

    public int getCharacteristicsSize() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getCharacteristicsSize");
        Log.i(TAG, "###################>");
        return mCharacteristics.values().size();
    }

    public Set<BluetoothGattDescriptorWrapper> getDescriptors() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getDescriptors");
        Log.i(TAG, "###################>");
        return new HashSet<BluetoothGattDescriptorWrapper>(mDescriptors.values());
    }

    /*public BluetoothGattDescriptorWrapper getDescriptor(String uuid) {
        Log.i(TAG, "getDescriptor");
        return mDescriptors.get(uuid);
    }*/

public BluetoothGattDescriptorWrapper getDescriptor(int id) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getDescriptor");
        Log.i(TAG, "###################>");
        return mDescriptors.get(id);
    }

    public void addDescriptor(BluetoothGattDescriptor descriptor) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "addDescriptors");
        Log.i(TAG, "###################>");
        mDescriptors.put(descriptor.hashCode(),
            BluetoothGattDescriptorWrapper.create(descriptor, this));
    }

    public int getDescriptorsSize() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getDescriptorsSize");
        Log.i(TAG, "###################>");
        return mServices.values().size();
    }
}
