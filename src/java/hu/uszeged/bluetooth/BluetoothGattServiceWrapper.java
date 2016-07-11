package hu.uszeged.bluetooth;

import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.util.Log;
import java.util.Set;
import java.util.HashSet;
import java.util.UUID;

final class BluetoothGattServiceWrapper {
    private static final String TAG = "BluetoothGattServiceWrapper";

    private BluetoothGattService mService;
    private BluetoothDeviceWrapper mDevice;
    private int mId;

    public BluetoothGattServiceWrapper(BluetoothGattService service,
        BluetoothDeviceWrapper device, int id) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "ctor");
        Log.i(TAG, "###################>");
        mService = service;
        mDevice = device;
        mId = id;
        Log.i(TAG, "Service: "+mService.getUuid().toString());
    }

    public static BluetoothGattServiceWrapper create(BluetoothGattService service,
        BluetoothDeviceWrapper device, int id) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "create");
        Log.i(TAG, "###################>");
        return new BluetoothGattServiceWrapper(service, device, id);
    }

    public BluetoothGattService get() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "get");
        Log.i(TAG, "###################>");
        return mService;
    }

    public int getId() {
        return mId;
    }

    public String getUuid() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getUuid "+mService.getUuid().toString());
        Log.i(TAG, "###################>");
        return mService.getUuid().toString();
    }

    public int getInstanceId() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getInstanceId "+mService.getInstanceId());
        Log.i(TAG, "###################>");
        return mService.getInstanceId();
    }

    public int getType() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getType "+mService.getType());
        Log.i(TAG, "###################>");
        return mService.getType();
    }

    public boolean isPrimary() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "isPrimary "+ (getType() == mService.SERVICE_TYPE_PRIMARY));
        Log.i(TAG, "###################>");
        return getType() == mService.SERVICE_TYPE_PRIMARY;
    }

    //TODO Set -> List
    public Set<BluetoothGattCharacteristicWrapper> getCharacteristics() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getCharacteristics");
        Log.i(TAG, "###################>");
        for (BluetoothGattCharacteristic characteristics : mService.getCharacteristics()) {
            mDevice.addCharacteristic(characteristics);
        }
        return mDevice.getCharacteristics();
    }

    public BluetoothGattCharacteristicWrapper getCharacteristic(String uuid) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getCharacteristic "+uuid);
        Log.i(TAG, "###################>");
        for (BluetoothGattCharacteristicWrapper characteristic : mDevice.getCharacteristics()) {
            if (characteristic.getUuid() == uuid) {
                return characteristic;           
            }
        }
        return null;
    }

    public BluetoothGattCharacteristicWrapper getCharacteristic(int id) {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getCharacteristic "+id);
        Log.i(TAG, "###################>");
        return mDevice.getCharacteristic(id);
    }

    public int getCharacteristicsSize() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getCharacteristicsSize");
        Log.i(TAG, "###################>");
        return mDevice.getCharacteristicsSize();
    }

    //TODO Set -> List
    public Set<BluetoothGattServiceWrapper> getIncludedServices() {
        Log.i(TAG, "###################<");
        Log.i(TAG, "getIncludedServices");
        Log.i(TAG, "###################>");
        Set<BluetoothGattServiceWrapper> result
            = new HashSet<BluetoothGattServiceWrapper>();
        for (BluetoothGattService service : mService.getIncludedServices()) {
            result.add(BluetoothGattServiceWrapper.create(service, mDevice, service.hashCode()));
        }
        return result;
    }
}
