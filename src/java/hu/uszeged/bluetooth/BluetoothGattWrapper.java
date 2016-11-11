package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattService;
import java.util.Set;
import java.util.HashSet;
import java.util.HashMap;

final class BluetoothGattWrapper {
    private BluetoothGatt mGatt;
    private BluetoothDeviceWrapper mDevice;
    private HashMap<Integer, BluetoothGattServiceWrapper> mServices;

    public BluetoothGattWrapper(BluetoothGatt gatt, BluetoothDeviceWrapper device) {
        mGatt = gatt;
        mDevice = device;
        mServices = new HashMap<Integer, BluetoothGattServiceWrapper>();
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

    public BluetoothGattServiceWrapper getService(int id) {
        return mServices.get(id);
    }

    public Set<BluetoothGattServiceWrapper> getServices() {
        for (BluetoothGattService service : mGatt.getServices()) {
            addService(service);
        }
        return new HashSet<BluetoothGattServiceWrapper>(mServices.values());
    }

    public void addService(BluetoothGattService service) {
        if (!mServices.containsKey(service.hashCode()))
            mServices.put(service.hashCode(),
                BluetoothGattServiceWrapper.create(service, mDevice));
    }

    public int getServicesSize() {
        return mServices.values().size();
    }

    public boolean readCharacteristic(BluetoothGattCharacteristicWrapper characteristic) {
        return !mDevice.isConnected() ? false :
            mGatt.readCharacteristic(characteristic.get());
    }

    public boolean readDescriptor(BluetoothGattDescriptorWrapper descriptor) {
        return !mDevice.isConnected() ? false :
            mGatt.readDescriptor(descriptor.get());
    }

    public boolean writeCharacteristic(BluetoothGattCharacteristicWrapper characteristic) {
        return !mDevice.isConnected() ? false :
            mGatt.writeCharacteristic(characteristic.get());
    }

    public boolean writeDescriptor(BluetoothGattDescriptorWrapper descriptor) {
        return !mDevice.isConnected() ? false :
            mGatt.writeDescriptor(descriptor.get());
    }

    public boolean setCharacteristicNotification(BluetoothGattCharacteristicWrapper characteristic, boolean enable) {
        return !mDevice.isConnected() ? false :
            mGatt.setCharacteristicNotification(characteristic.get(), enable);
    }
}
