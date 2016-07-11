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
        Log.i(TAG, "###################<");
        Log.i(TAG, "ctor");
        Log.i(TAG, "###################>");
        mGatt = gatt;
        mDevice = device;
    }

    public static BluetoothGattWrapper create(BluetoothGatt gatt, BluetoothDeviceWrapper device) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "create");
        Log.i(TAG, "###################>");
        return new BluetoothGattWrapper(gatt, device);
    }

    public BluetoothGatt get() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "get");
        Log.i(TAG, "###################>");
        return mGatt;
    }

    public BluetoothDeviceWrapper device() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "device");
        Log.i(TAG, "###################>");
        return mDevice;
    }

    public boolean connect() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "connect");
        Log.i(TAG, "###################>");
        return mGatt.connect();
    }

    public void disconnect() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "disconnect");
        Log.i(TAG, "###################>");
        mGatt.disconnect();
    }

    public boolean discoverServices() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "discoverServices");
        Log.i(TAG, "###################>");
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
        Log.i(TAG, "###################<");
        Log.i(TAG, "getService");
        Log.i(TAG, "###################>");
        for (BluetoothGattServiceWrapper service : mDevice.getServices()) {
            if (service.getUuid() == uuid) {
                return service;           
            }
        }
        return null;
    }

    public BluetoothGattServiceWrapper getService(int id) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getService");
        Log.i(TAG, "###################>");
        return mDevice.getService(id);
    }

    //TODO Set -> List
    public Set<BluetoothGattServiceWrapper> getServices() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getServices");
        Log.i(TAG, "###################>");
        for (BluetoothGattService service : mGatt.getServices()) {
            mDevice.addService(service);
        }
        return mDevice.getServices();
    }

    public boolean readCharacteristic(BluetoothGattCharacteristicWrapper characteristic) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "readCharacteristic");
        Log.i(TAG, "###################>");
        return mGatt.readCharacteristic(
            characteristic.get());
    }

    public boolean readDescriptor(BluetoothGattDescriptorWrapper descriptor) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "readDescriptor");
        Log.i(TAG, "###################>");
        return mGatt.readDescriptor(
            descriptor.get());
    }

    public boolean writeCharacteristic(BluetoothGattCharacteristicWrapper characteristic) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "writeCharacteristic");
        Log.i(TAG, "###################>");
        return mGatt.writeCharacteristic(
            characteristic.get());
    }

    public boolean writeCharacteristic(BluetoothGattCharacteristicWrapper characteristic, byte[] values) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "writeCharacteristic");
        Log.i(TAG, "###################>");
        characteristic.setValue(values);
        return mGatt.writeCharacteristic(
            characteristic.get());
    }

    public boolean writeDescriptor(BluetoothGattDescriptorWrapper descriptor) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "writeDescriptor");
        Log.i(TAG, "###################>");
        return mGatt.writeDescriptor(
            descriptor.get());
    }

    public boolean writeDescriptor(BluetoothGattDescriptorWrapper descriptor, byte[] values) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "writeDescriptor");
        Log.i(TAG, "###################>");
        descriptor.setValue(values);
        return mGatt.writeDescriptor(
            descriptor.get());
    }
}
