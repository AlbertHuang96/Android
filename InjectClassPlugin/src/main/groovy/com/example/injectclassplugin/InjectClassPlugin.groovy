package com.example.injectclassplugin

import com.android.build.gradle.AppExtension
import org.gradle.api.Plugin
import org.gradle.api.Project

class InjectClassPlugin implements Plugin<Project> {

    @Override
    void apply(Project project) {
        def android = project.extensions.getByType(AppExtension.class)
        def classTransform = new InjectClassTransform(project)
        android.registerTransform(classTransform)

        def extension = project.extensions.create("InjectClassCode", InjectClassExtension)
        project.afterEvaluate {
            classTransform.injectCode = extension.injectCode
        }
    }
}
