package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattService;
import java.util.Set;
import java.util.HashSet;

final class BluetoothGattWrapper {
    private BluetoothGatt mGatt;
    private BluetoothDeviceWrapper mDevice;

    public BluetoothGattWrapper(BluetoothGatt gatt, BluetoothDeviceWrapper device) {
        mGatt = gatt;
        mDevice = device;
    }

    public static BluetoothGattWrapper create(BluetoothGatt gatt, BluetoothDeviceWrapper device) {
        return new BluetoothGattWrapper(gatt, device);
    }

    public BluetoothGatt get() {
        return mGatt;
    }

    public BluetoothDeviceWrapper device() {
        return mDevice;
    }

    public boolean connect() {
        return mGatt.connect();
    }

    public void disconnect() {
        mGatt.disconnect();
    }

    public boolean discoverServices() {
        return mGatt.discoverServices();
    }

    public BluetoothGattServiceWrapper getService(String uuid) {
        for (BluetoothGattServiceWrapper service : mDevice.getServices()) {
            if (service.getUuid() == uuid)
                return service;
        }
        return null;
    }

    public BluetoothGattServiceWrapper getService(int id) {
        return mDevice.getService(id);
    }

    public Set<BluetoothGattServiceWrapper> getServices() {
        for (BluetoothGattService service : mGatt.getServices()) {
            mDevice.addService(service);
        }
        return mDevice.getServices();
    }

    public int getServicesSize() {
        return mDevice.getServicesSize();
    }

    public boolean readCharacteristic(BluetoothGattCharacteristicWrapper characteristic) {
        return mGatt.readCharacteristic(characteristic.get());
    }

    public boolean readDescriptor(BluetoothGattDescriptorWrapper descriptor) {
        return mGatt.readDescriptor(descriptor.get());
    }

    public boolean writeCharacteristic(BluetoothGattCharacteristicWrapper characteristic) {
        return mGatt.writeCharacteristic(characteristic.get());
    }

    public boolean writeDescriptor(BluetoothGattDescriptorWrapper descriptor) {
        return mGatt.writeDescriptor(descriptor.get());
    }
}
