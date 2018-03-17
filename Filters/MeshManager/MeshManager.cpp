//
// Created by paul on 15.03.18.
//

#include <iostream>
#include "MeshManager.hpp"

Channel<rcLib::Package> &MeshManager::getLoraIn() {
    return loraIn;
}

Channel<rcLib::Package> &MeshManager::getSerialIn() {
    return serialIn;
}

Channel<rcLib::Package> &MeshManager::getLoraOut() {
    return loraOut;
}

Channel<rcLib::Package> &MeshManager::getSerialOut() {
    return serialOut;
}

void MeshManager::run() {
    while(!loraIn.isClosed() || !serialIn.isClosed() || !loraOut.isClosed() || !serialOut.isClosed()) {
        rcLib::Package pkg;
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
