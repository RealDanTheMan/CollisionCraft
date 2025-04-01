//
// Copyright 2018 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#ifndef PXR_USD_SDR_SHADER_PROPERTY_H
#define PXR_USD_SDR_SHADER_PROPERTY_H

/// \file sdr/shaderProperty.h
///
/// \note
/// All Ndr objects are deprecated in favor of the corresponding Sdr objects
/// in this file. All existing pxr/usd/ndr implementations will be moved to
/// pxr/usd/sdr.

#include "pxr/pxr.h"
#include "pxr/base/tf/staticTokens.h"
#include "pxr/base/tf/token.h"
#include "pxr/base/tf/weakBase.h"
#include "pxr/base/vt/value.h"
#include "pxr/usd/ndr/property.h"
#include "pxr/usd/sdr/api.h"
#include "pxr/usd/sdr/declare.h"
#include "pxr/usd/sdr/sdfTypeIndicator.h"
#include "pxr/usd/sdr/shaderNode.h"

PXR_NAMESPACE_OPEN_SCOPE

// If additional types are added here, it's also worth trying to add a mapping
// to the equivalent Sdf type in the implementation file.
#define SDR_PROPERTY_TYPE_TOKENS \
    ((Int,      "int"))          \
    ((String,   "string"))       \
    ((Float,    "float"))        \
    ((Color,    "color"))        \
    ((Color4,   "color4"))       \
    ((Point,    "point"))        \
    ((Normal,   "normal"))       \
    ((Vector,   "vector"))       \
    ((Matrix,   "matrix"))       \
    ((Struct,   "struct"))       \
    ((Terminal, "terminal"))     \
    ((Vstruct,  "vstruct"))      \
    ((Unknown,  "unknown"))

// Note: Metadata keys that are generated by parsers should start with
// "__SDR__" to reduce the risk of collision with metadata actually in the
// shader.
#define SDR_PROPERTY_METADATA_TOKENS                 \
    ((Label, "label"))                               \
    ((Help, "help"))                                 \
    ((Page, "page"))                                 \
    ((RenderType, "renderType"))                     \
    ((Role, "role"))                                 \
    ((Widget, "widget"))                             \
    ((Hints, "hints"))                               \
    ((Options, "options"))                           \
    ((IsDynamicArray, "isDynamicArray"))             \
    ((Connectable, "connectable"))                   \
    ((Tag, "tag"))                                   \
    ((ValidConnectionTypes, "validConnectionTypes")) \
    ((VstructMemberOf, "vstructMemberOf"))           \
    ((VstructMemberName, "vstructMemberName"))       \
    ((VstructConditionalExpr, "vstructConditionalExpr"))\
    ((IsAssetIdentifier, "__SDR__isAssetIdentifier"))\
    ((ImplementationName, "__SDR__implementationName"))\
    ((SdrUsdDefinitionType, "sdrUsdDefinitionType"))\
    ((DefaultInput, "__SDR__defaultinput"))          \
    ((Target, "__SDR__target"))                      \
    ((Colorspace, "__SDR__colorspace"))


// The following tokens are valid values for the metadata "role"
#define SDR_PROPERTY_ROLE_TOKENS \
    ((None, "none"))

#define SDR_PROPERTY_TOKENS \
    ((PageDelimiter, ":"))

TF_DECLARE_PUBLIC_TOKENS(SdrPropertyTypes, SDR_API, SDR_PROPERTY_TYPE_TOKENS);
TF_DECLARE_PUBLIC_TOKENS(SdrPropertyMetadata, SDR_API, 
                         SDR_PROPERTY_METADATA_TOKENS);
TF_DECLARE_PUBLIC_TOKENS(SdrPropertyRole, SDR_API,
                         SDR_PROPERTY_ROLE_TOKENS);
TF_DECLARE_PUBLIC_TOKENS(SdrPropertyTokens, SDR_API, SDR_PROPERTY_TOKENS);

/// \class SdrShaderProperty
///
/// Represents a property (input or output) that is part of a `SdrShaderNode`
/// instance.
///
/// A property must have a name and type, but may also specify a host of
/// additional metadata. Instances can also be queried to determine if another
/// `SdrShaderProperty` instance can be connected to it.
///
class SdrShaderProperty : public NdrProperty
{
public:
    // Constructor.
    SDR_API
    SdrShaderProperty(
        const TfToken& name,
        const TfToken& type,
        const VtValue& defaultValue,
        bool isOutput,
        size_t arraySize,
        const SdrTokenMap& metadata,
        const SdrTokenMap& hints,
        const SdrOptionVec& options
    );

    /// \name Metadata
    /// The metadata returned here is a direct result of what the parser plugin
    /// is able to determine about the property. See the documentation for a
    /// specific parser plugin to get help on what the parser is looking for to
    /// populate these values.
    /// @{

    /// The label assigned to this property, if any. Distinct from the name
    /// returned from `GetName()`. In the context of a UI, the label value
    /// might be used as the display name for the property instead of the name.
    SDR_API
    const TfToken& GetLabel() const { return _label; }

    /// The help message assigned to this property, if any.
    SDR_API
    std::string GetHelp() const;

    /// The page (group), eg "Advanced", this property appears on, if any. Note
    /// that the page for a shader property can be nested, delimited by ":", 
    /// representing the hierarchy of sub-pages a property is defined in.
    SDR_API
    const TfToken& GetPage() const { return _page; }

    /// The widget "hint" that indicates the widget that can best display the
    /// type of data contained in this property, if any. Examples of this value
    /// could include "number", "slider", etc.
    SDR_API
    const TfToken& GetWidget() const { return _widget; }

    /// Any UI "hints" that are associated with this property. "Hints" are
    /// simple key/value pairs.
    SDR_API
    const SdrTokenMap& GetHints() const { return _hints; }

    /// If the property has a set of valid values that are pre-determined, this
    /// will return the valid option names and corresponding string values (if
    /// the option was specified with a value).
    SDR_API
    const SdrOptionVec& GetOptions() const { return _options; }

    /// Returns the implementation name of this property.  The name of the
    /// property is how to refer to the property in shader networks.  The
    /// label is how to present this property to users.  The implementation
    /// name is the name of the parameter this property represents in the
    /// implementation.  Any client using the implementation \b must call
    /// this method to get the correct name;  using \c getName() is not
    /// correct.
    SDR_API
    std::string GetImplementationName() const;

    /// @}


    /// \name VStruct Information
    /// @{

    /// If this field is part of a vstruct, this is the name of the struct.
    SDR_API
    const TfToken& GetVStructMemberOf() const {
        return _vstructMemberOf;
    }

    /// If this field is part of a vstruct, this is its name in the struct.
    SDR_API
    const TfToken& GetVStructMemberName() const {
        return _vstructMemberName;
    }

    /// Returns true if this field is part of a vstruct.
    SDR_API
    bool IsVStructMember() const;

    /// Returns true if the field is the head of a vstruct.
    SDR_API
    bool IsVStruct() const;


    /// If this field is part of a vstruct, this is the conditional expression
    SDR_API
    const TfToken& GetVStructConditionalExpr() const {
        return _vstructConditionalExpr;
    }

    /// @}


    /// \name Connection Information
    /// @{

    /// Whether this property can be connected to other properties. If this
    /// returns `true`, connectability to a specific property can be tested via
    /// `CanConnectTo()`.
    SDR_API
    bool IsConnectable() const override { return _isConnectable; }

    /// Gets the list of valid connection types for this property. This value
    /// comes from shader metadata, and may not be specified. The value from
    /// `SdrShaderProperty::GetType()` can be used as a fallback, or you can
    /// use the connectability test in `CanConnectTo()`.
    SDR_API
    const SdrTokenVec& GetValidConnectionTypes() const {
        return _validConnectionTypes;
    }

    /// Determines if this property can be connected to the specified property.
    ///
    /// \deprecated
    /// Deprecated in favor of
    /// SdrShaderProperty::CanConnectTo(SdrShaderProperty)
    SDR_API
    bool CanConnectTo(const NdrProperty& other) const override;

    /// Determines if this property can be connected to the specified property.
    SDR_API
    bool CanConnectTo(const SdrShaderProperty& other) const;

    /// @}


    /// \name Utilities
    /// @{

    /// Converts the property's type from `GetType()` into a
    /// `SdrSdfTypeIndicator`.
    ///
    /// Two scenarios can result: an exact mapping from property type to Sdf
    /// type, and an inexact mapping. In the first scenario,
    /// SdrSdfTypeIndicator will contain a cleanly-mapped Sdf type. In the
    /// second scenario, the SdrSdfTypeIndicator will contain an Sdf type
    /// set to `Token` to indicate an unclean mapping, and
    /// SdrSdfTypeIndicator::GetSdrType will be set to the original type
    /// returned by `GetType()`.
    SDR_API
    SdrSdfTypeIndicator GetTypeAsSdfType() const override;

    /// Accessor for default value corresponding to the SdfValueTypeName
    /// returned by GetTypeAsSdfType. Note that this is different than 
    /// GetDefaultValue which returns the default value associated with the 
    /// SdrPropertyType and may differ from the SdfValueTypeName, example when
    /// sdrUsdDefinitionType metadata is specified for a sdr property.
    ///
    /// \sa GetTypeAsSdfType
    SDR_API
    const VtValue& GetDefaultValueAsSdfType() const override {
        return _sdfTypeDefaultValue;
    }

    /// Determines if the value held by this property is an asset identifier
    /// (eg, a file path); the logic for this is left up to the parser.
    ///
    /// Note: The type returned from `GetTypeAsSdfType()` will be `Asset` if
    /// this method returns `true` (even though its true underlying data type
    /// is string).
    SDR_API
    bool IsAssetIdentifier() const;

    /// Determines if the value held by this property is the default input 
    /// for this node.
    SDR_API
    bool IsDefaultInput() const;

    /// @}

protected:
    SdrShaderProperty& operator=(const SdrShaderProperty&) = delete;

    // Allow the shader's post process function to access the property's
    // internals. Some property information can only be determined after parse
    // time.
    friend void SdrShaderNode::_PostProcessProperties();

    // Set the USD encoding version to something other than the default.
    // This can be set in SdrShaderNode::_PostProcessProperties for all the
    // properties on a shader node.
    void _SetUsdEncodingVersion(int usdEncodingVersion);

    // Convert this property to a VStruct, which has a special type and a
    // different default value
    void _ConvertToVStruct();

    // This function is called by SdrShaderNode::_PostProcessProperties once all
    // information is locked in and won't be changed anymore. This allows each
    // property to take some extra steps once all information is available.
    void _FinalizeProperty();

    // Some metadata values cannot be returned by reference from the main
    // metadata dictionary because they need additional parsing.
    const SdrTokenMap _hints;
    const SdrOptionVec _options;

    // Tokenized metadata
    SdrTokenVec _validConnectionTypes;
    TfToken _label;
    TfToken _page;
    TfToken _widget;
    TfToken _vstructMemberOf;
    TfToken _vstructMemberName;
    TfToken _vstructConditionalExpr;

    VtValue _sdfTypeDefaultValue;

    // Metadatum to control the behavior of GetTypeAsSdfType and indirectly
    // CanConnectTo
    int _usdEncodingVersion;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif // PXR_USD_SDR_SHADER_PROPERTY_H
