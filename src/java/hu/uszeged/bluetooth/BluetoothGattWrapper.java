package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattService;
import android.util.Log;
import java.util.Collections;
import java.util.Set;
import java.util.HashSet;
import java.util.UUID;

final class BluetoothGattWrapper {
    private static final String TAG = "BluetoothGattWrapper";

    private BluetoothGatt mGatt;
    private BluetoothDeviceWrapper mDevice;

    public BluetoothGattWrapper(BluetoothGatt gatt, BluetoothDeviceWrapper device) {
        Log.i(TAG, "ctor");
        mGatt = gatt;
        mDevice = device;
    }

    public static BluetoothGattWrapper create(BluetoothGatt gatt, BluetoothDeviceWrapper device) {
        Log.i(TAG, "create");
        return new BluetoothGattWrapper(gatt, device);
    }

    public BluetoothGatt get() {
        Log.i(TAG, "get");
        return mGatt;
    }

    public BluetoothDeviceWrapper device() {
        Log.i(TAG, "device");
        return mDevice;
    }

    public boolean connect() {
        Log.i(TAG, "connect");
        return mGatt.connect();
    }

    public void disconnect() {
        Log.i(TAG, "disconnect");
        mGatt.disconnect();
    }

    public boolean discoverServices() {
        Log.i(TAG, "discoverServices");
        boolean res = mGatt.discoverServices();
        /*try {
            Log.i(TAG, "wait 1 sec...");
            java.lang.Thread.sleep(1000);
        } catch (Exception e) {
            System.out.println(e);
        }*/
        return res;
    }

    public BluetoothGattServiceWrapper getService(String uuid) {
        Log.i(TAG, "getService");
        for (BluetoothGattServiceWrapper service : mDevice.getServices()) {
            if (service.getUuid() == uuid) {
                return service;           
            }
        }
        return null;
    }

    public BluetoothGattServiceWrapper getService(int id) {
        Log.i(TAG, "getService");
        return mDevice.getService(id);
    }

    //TODO Set -> List
    public Set<BluetoothGattServiceWrapper> getServices() {
        Log.i(TAG, "getServices");
        for (BluetoothGattService service : mGatt.getServices()) {
            mDevice.addService(service);
        }
        return mDevice.getServices();
    }

    public boolean readCharacteristic(BluetoothGattCharacteristicWrapper characteristic) {
        Log.i(TAG, "readCharacteristic");
        return mGatt.readCharacteristic(
            characteristic.get());
    }

    public boolean readDescriptor(BluetoothGattDescriptorWrapper descriptor) {
        Log.i(TAG, "readDescriptor");
        return mGatt.readDescriptor(
            descriptor.get());
    }

    public boolean writeCharacteristic(BluetoothGattCharacteristicWrapper characteristic) {
        Log.i(TAG, "writeCharacteristic");
        return mGatt.writeCharacteristic(
            characteristic.get());
    }

    public boolean writeCharacteristic(BluetoothGattCharacteristicWrapper characteristic, byte[] values) {
        Log.i(TAG, "writeCharacteristic");
        characteristic.setValue(values);
        return mGatt.writeCharacteristic(
            characteristic.get());
    }

    public boolean writeDescriptor(BluetoothGattDescriptorWrapper descriptor) {
        Log.i(TAG, "writeDescriptor");
        return mGatt.writeDescriptor(
            descriptor.get());
    }

    public boolean writeDescriptor(BluetoothGattDescriptorWrapper descriptor, byte[] values) {
        Log.i(TAG, "writeDescriptor");
        descriptor.setValue(values);
        return mGatt.writeDescriptor(
            descriptor.get());
    }
}
