

package com.example.printdependency

import org.gradle.api.provider.Property

interface DependenciesPluginExtension {
    Property<Boolean> getEnable();
}