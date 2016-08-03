package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import java.util.Set;
import java.util.HashSet;

final class BluetoothGattServiceWrapper {
    private BluetoothGattService mService;
    private BluetoothDeviceWrapper mDevice;
    private int mId;

    public BluetoothGattServiceWrapper(BluetoothGattService service,
            BluetoothDeviceWrapper device) {
        mService = service;
        mDevice = device;
        mId = service.hashCode();
    }

    public static BluetoothGattServiceWrapper create(BluetoothGattService service,
            BluetoothDeviceWrapper device) {
        return new BluetoothGattServiceWrapper(service, device);
    }

    public BluetoothGattService get() {
        return mService;
    }

    public int getId() {
        return mId;
    }

    public String getUuid() {
        return mService.getUuid().toString();
    }

    public int getInstanceId() {
        return mService.getInstanceId();
    }

    public int getType() {
        return mService.getType();
    }

    public boolean isPrimary() {
        return getType() == mService.SERVICE_TYPE_PRIMARY;
    }

    public Set<BluetoothGattCharacteristicWrapper> getCharacteristics() {
        for (BluetoothGattCharacteristic characteristics : mService.getCharacteristics()) {
            mDevice.addCharacteristic(characteristics);
        }
        return mDevice.getCharacteristics();
    }

    public BluetoothGattCharacteristicWrapper getCharacteristic(String uuid) {
        for (BluetoothGattCharacteristicWrapper characteristic : mDevice.getCharacteristics()) {
            if (characteristic.getUuid() == uuid)
                return characteristic;
        }
        return null;
    }

    public BluetoothGattCharacteristicWrapper getCharacteristic(int id) {
        return mDevice.getCharacteristic(id);
    }

    public int getCharacteristicsSize() {
        return mDevice.getCharacteristicsSize();
    }

    public Set<BluetoothGattServiceWrapper> getIncludedServices() {
        Set<BluetoothGattServiceWrapper> result
            = new HashSet<BluetoothGattServiceWrapper>();
        for (BluetoothGattService service : mService.getIncludedServices()) {
            result.add(BluetoothGattServiceWrapper.create(service, mDevice));
        }
        return result;
    }
}
