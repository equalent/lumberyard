/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
#include <AWS_precompiled.h>
#include <Configuration/FlowNode_ConfigureParameterValue.h>

#include <CloudCanvas/CloudCanvasMappingsBus.h>
#include <CloudCanvas/CloudCanvasIdentityBus.h>

namespace LmbrAWS
{
    FlowNode_ConfigureParameterValue::FlowNode_ConfigureParameterValue(IFlowNode::SActivationInfo* activationInfo)
        : BaseMaglevFlowNode < eNCT_Singleton >(activationInfo)
    {
    }
    Aws::Vector<SInputPortConfig> FlowNode_ConfigureParameterValue::GetInputPorts() const
    {
        static const Aws::Vector<SInputPortConfig> inputPortConfiguration =
        {
            InputPortConfig_Void("Configure", "Set the parameter value."),
            InputPortConfig<string>("Name", "Name of parameter to set."),
            InputPortConfig<string>("Value", "Value to which parameter will be set. May contain $param-name$ substrings.")
        };

        return inputPortConfiguration;
    }

    Aws::Vector<SOutputPortConfig> FlowNode_ConfigureParameterValue::GetOutputPorts() const
    {
        static Aws::Vector<SOutputPortConfig> outputPortConfiguration =
        {
        };
        return outputPortConfiguration;
    }


    void FlowNode_ConfigureParameterValue::ProcessEvent_Internal(EFlowEvent event, SActivationInfo* pActInfo)
    {
        if (event == eFE_Activate && IsPortActive(pActInfo, EIP_Configure))
        {
            auto name = GetPortString(pActInfo, EIP_Name);
            auto value = GetPortString(pActInfo, EIP_Value);
            
            EBUS_EVENT(CloudGemFramework::CloudCanvasMappingsBus, SetLogicalMapping, "", name.c_str(), value.c_str());

            EBUS_EVENT(CloudGemFramework::CloudCanvasPlayerIdentityBus, ApplyConfiguration);

            ActivateOutput(pActInfo, EOP_Success, true);
        }
    }

    REGISTER_CLASS_TAG_AND_FLOW_NODE("AWS:Configuration:ConfigureParameterValue", FlowNode_ConfigureParameterValue);
} // namespace Amazon
