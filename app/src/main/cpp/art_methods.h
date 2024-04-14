//
// Created by 86422 on 2024/4/5.
//

#ifndef RENDERPLAYGROUND_ART_METHODS_H
#define RENDERPLAYGROUND_ART_METHODS_H

//https://android.googlesource.com/platform/art/+/refs/tags/android-platform-12.0.0_r30/runtime/art_method.h
namespace art {
    namespace mirror {
        class Array;
        class Class;
        class ClassLoader;
        class DexCache;
        class IfTable;
        class Object;
        template <typename MirrorType> class ObjectArray;
        class PointerArray;
        class String;
        template <typename T> struct NativeDexCachePair;
        //using MethodDexCachePair = NativeDexCachePair<ArtMethod>;
        //using MethodDexCacheType = std::atomic<MethodDexCachePair>;
    }  // namespace mirror

    class ArtMethod {
    public:
        //GcRoot<mirror::Class> declaring_class_;
        uint32_t declaring_class_;

        // Access flags; low 16 bits are defined by spec.
        // Getting and setting this flag needs to be atomic when concurrency is
        // possible, e.g. after this method's class is linked. Such as when setting
        // verifier flags and single-implementation flag.
        std::atomic<std::uint32_t> access_flags_;
        /* Dex file fields. The defining dex file is available via declaring_class_->dex_cache_ */
        // Index into method_ids of the dex file associated with this method.
        uint32_t dex_method_index_;
        /* End of dex file fields. */
        // Entry within a dispatch table for this method. For static/direct methods the index is into
        // the declaringClass.directMethods, for virtual methods the vtable and for interface methods the
        // ifTable.
        uint16_t method_index_;
        union {
            // Non-abstract methods: The hotness we measure for this method. Not atomic,
            // as we allow missing increments: if the method is hot, we will see it eventually.
            uint16_t hotness_count_;
            // Abstract methods: IMT index.
            uint16_t imt_index_;
        };
        // Fake padding field gets inserted here.
        // Must be the last fields in the method.
        struct PtrSizedFields {
            // Depending on the method type, the data is
            //   - native method: pointer to the JNI function registered to this method
            //                    or a function to resolve the JNI function,
            //   - resolution method: pointer to a function to resolve the method and
            //                        the JNI function for @CriticalNative.
            //   - conflict method: ImtConflictTable,
            //   - abstract/interface method: the single-implementation if any,
            //   - proxy method: the original interface method or constructor,
            //   - other methods: during AOT the code item offset, at runtime a pointer
            //                    to the code item.
            void* data_;
            // Method dispatch from quick compiled code invokes this pointer which may cause bridging into
            // the interpreter.
            void* entry_point_from_quick_compiled_code_;
        } ptr_sized_fields_;
    };
}

#endif //RENDERPLAYGROUND_ART_METHODS_H
