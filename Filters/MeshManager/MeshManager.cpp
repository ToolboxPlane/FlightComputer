//
// Created by paul on 15.03.18.
//

#include <iostream>
#include "MeshManager.hpp"

Channel<rcLib::PackageExtended> &MeshManager::getLoraIn() {
    return loraIn;
}

Channel<rcLib::PackageExtended> &MeshManager::getSerialIn() {
    return serialIn;
}

Channel<rcLib::PackageExtended> &MeshManager::getLoraOut() {
    return loraOut;
}

Channel<rcLib::PackageExtended> &MeshManager::getSerialOut() {
    return serialOut;
}

void MeshManager::run() {
    while(!loraIn.isClosed() || !serialIn.isClosed() || !loraOut.isClosed() || !serialOut.isClosed()) {
        rcLib::PackageExtended pkg;
        if(loraIn.get(pkg, false)) {
            if(pkg.needsForwarding()) {
                pkg.countNode();
                serialOut.put(pkg);
            }
        }
        if(serialIn.get(pkg, false)) {
            if(pkg.needsForwarding())  {
                pkg.countNode();
                loraOut.put(pkg);
            }
        }
    }
}

MeshManager::MeshManager() : ThreadModule(){
}
