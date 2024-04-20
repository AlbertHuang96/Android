
package com.example.printdependency

import org.gradle.api.Plugin
import org.gradle.api.Project
import com.android.build.gradle.AppExtension
import org.gradle.api.artifacts.ConfigurationContainer
import org.gradle.api.artifacts.Dependency
import org.gradle.api.artifacts.ModuleVersionIdentifier

import com.example.printdependency.DependenciesPluginExtension

import org.gradle.api.artifacts.Configuration

// cmd to print dependencies: ./gradlew app:dependencies
class DependenciesPlugin implements Plugin<Project> {
    private final String TAG = "DependenciesPlugin >>>>"

    @Override
    public void apply(Project project) {
        System.out.println(TAG + this.getClass().getName())

        DependenciesPluginExtension extension = project.getExtensions().create("printDependencies", DependenciesPluginExtension.class)

        // we can get dependencies after configuartion
        project.afterEvaluate(pro -> {

            extension.getEnable().convention(false)
            if (extension.getEnable().get()) {
                System.out.println(TAG + "turn on the printing")
                AppExtension androidExt = project.getExtensions().getByType(AppExtension.class)
                androidExt.getApplicationVariants().all(applicationVariant -> {
                    System.out.println(TAG + "applicationVariant.getName = " + applicationVariant.getName())
                    //String nametmp = applicationVariant.getName()
                    //ConfigurationContainer ctner = project.getConfigurations()

                    /*project.getConfigurations().forEach(config -> {
                        System.out.println(TAG + "configuration = " + config.name)
                    })*/
                    Configuration configuration = project.getConfigurations().getByName(applicationVariant.getName() + "CompileClasspath")
                    Set<Dependency> allDependencies = configuration.getAllDependencies()
                    allDependencies.forEach(dependency -> {
                        System.out.println(TAG + "configuration.getAllDependencies() dependency = " + dependency.getGroup() + ":" + dependency.getName() + ":" + dependency.getVersion())
                    })
                    // iterate the allDependencies and print out the dependency

                    List<String> androidLibs = new ArrayList<>()
                    List<String> otherLibs = new ArrayList<>()
                    configuration.getResolvedConfiguration().getLenientConfiguration().getAllModuleDependencies().forEach(resolvedDependency -> {
                        ModuleVersionIdentifier identifier = resolvedDependency.getModule().getId()
                        if (identifier.getGroup().contains("androidx") || identifier.getGroup().contains("com.google") || identifier.getGroup().contains("org.jetbrains")) {
                            androidLibs.add(identifier.getGroup() + ":" + identifier.getName() + ":" + identifier.getVersion())
                        } else {
                            otherLibs.add(identifier.getGroup() + ":" + identifier.getName() + ":" + identifier.getVersion())
                        }
                    })

                    System.out.println("-----Official libs start-----")
                    androidLibs.forEach(System.out::println)
                    System.out.println("-----Official libs over-----")

                    System.out.println("-----Third libs start-----")
                    otherLibs.forEach(System.out::println)
                    System.out.println("-----Third libs over-----")

                })
            } else {
                System.out.println(TAG + "turn off the printing")
            }

        })
    }
}

/*
> Configure project :app

DependenciesPlugin >>>>turn on the printing
DependenciesPlugin >>>>applicationVariant.getName = debug
DependenciesPlugin >>>>configuration.getAllDependencies() dependency = androidx.cardview:cardview:1.0.0
DependenciesPlugin >>>>configuration.getAllDependencies() dependency = androidx.appcompat:appcompat:1.2.0
DependenciesPlugin >>>>configuration.getAllDependencies() dependency = com.google.android.material:material:1.3.0
DependenciesPlugin >>>>configuration.getAllDependencies() dependency = androidx.constraintlayout:constraintlayout:2.0.4
-----Official libs start-----
com.google.android.material:material:1.3.0
androidx.cardview:cardview:1.0.0
androidx.constraintlayout:constraintlayout:2.0.4
androidx.appcompat:appcompat:1.2.0
androidx.viewpager2:viewpager2:1.0.0
androidx.fragment:fragment:1.1.0
androidx.coordinatorlayout:coordinatorlayout:1.1.0
androidx.dynamicanimation:dynamicanimation:1.0.0
androidx.recyclerview:recyclerview:1.1.0
androidx.transition:transition:1.2.0
androidx.vectordrawable:vectordrawable:1.1.0
androidx.core:core:1.3.1
androidx.lifecycle:lifecycle-runtime:2.1.0
androidx.annotation:annotation:1.1.0
androidx.annotation:annotation-experimental:1.0.0
androidx.constraintlayout:constraintlayout-solver:2.0.4
androidx.appcompat:appcompat-resources:1.2.0
androidx.drawerlayout:drawerlayout:1.0.0
androidx.cursoradapter:cursoradapter:1.0.0
androidx.collection:collection:1.1.0
androidx.viewpager:viewpager:1.0.0
androidx.loader:loader:1.0.0
androidx.activity:activity:1.0.0
androidx.lifecycle:lifecycle-viewmodel:2.1.0
androidx.customview:customview:1.0.0
androidx.legacy:legacy-support-core-utils:1.0.0
androidx.versionedparcelable:versionedparcelable:1.1.0
androidx.lifecycle:lifecycle-common:2.1.0
androidx.arch.core:core-common:2.1.0
androidx.vectordrawable:vectordrawable-animated:1.1.0
androidx.lifecycle:lifecycle-livedata:2.0.0
androidx.savedstate:savedstate:1.0.0
androidx.documentfile:documentfile:1.0.0
androidx.localbroadcastmanager:localbroadcastmanager:1.0.0
androidx.print:print:1.0.0
androidx.interpolator:interpolator:1.0.0
androidx.lifecycle:lifecycle-livedata-core:2.0.0
androidx.arch.core:core-runtime:2.0.0
-----Official libs over-----
-----Third libs start-----
-----Third libs over-----
 */
