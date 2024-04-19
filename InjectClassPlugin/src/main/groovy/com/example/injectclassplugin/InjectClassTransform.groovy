//import com.android.build.api.transform.QualifiedContent
//import com.android.build.api.transform.Transform
package com.example.injectclassplugin
import com.android.build.api.transform.*
import com.android.build.gradle.internal.pipeline.TransformManager
import org.gradle.api.Project
//import org.gradle.internal.impldep.org.apache.commons.codec.digest.DigestUtils
import org.apache.commons.codec.digest.DigestUtils
import org.apache.commons.io.FileUtils
//import org.gradle.internal.impldep.org.apache.commons.io.FileUtils


class InjectClassTransform extends Transform {
    Project project
    String injectCode

    InjectClassTransform(Project project) {
        this.project = project
    }

    @Override
    String getName() {
        return "PreDexInjectCode"
    }
    @Override
    Set<QualifiedContent.ContentType> getInputTypes() {
        return TransformManager.CONTENT_CLASS
    }

    @Override
    Set<? super QualifiedContent.Scope> getScopes() {
        return TransformManager.SCOPE_FULL_PROJECT
    }

    @Override
    boolean isIncremental() {
        return false
    }

    @Override
    void transform(TransformInvocation transformInvocation) throws TransformException, InterruptedException, IOException {
        TransformOutputProvider provider = transformInvocation.outputProvider;

        for (TransformInput input : transformInvocation.inputs) {
            if (input == null) continue

            for (DirectoryInput directoryInput : input.directoryInputs) {
                InjectClass.inject(directoryInput.file.absolutePath, project, injectCode)
                def dest = provider.getContentLocation(directoryInput.name, directoryInput.contentTypes, directoryInput.scopes, Format.DIRECTORY)
                FileUtils.copyDirectory(directoryInput.file, dest)
            }

            for (JarInput jarInput : input.jarInputs) {
                def jarName = jarInput.name
                def md5Name = DigestUtils.md5Hex(jarInput.file.getAbsolutePath())
                if (jarName.endsWith("jar")) {
                    jarName.substring(0, jarName.length() - 4)
                }
                def dest = provider.getContentLocation(jarName + md5Name, jarInput.contentTypes, jarInput.scopes, Format.JAR)
                FileUtils.copyFile(jarInput.file, dest)
            }
        }

    }

}
