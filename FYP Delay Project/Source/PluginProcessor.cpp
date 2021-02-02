/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FYPDelayProjectAudioProcessor::FYPDelayProjectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ) , apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
    lowPassFilter.add(new juce::dsp::FirstOrderTPTFilter<float>());
    lowPassFilter[0]->setType(juce::dsp::FirstOrderTPTFilterType::lowpass);
}

FYPDelayProjectAudioProcessor::~FYPDelayProjectAudioProcessor()
{
}

//==============================================================================
const juce::String FYPDelayProjectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FYPDelayProjectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FYPDelayProjectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FYPDelayProjectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FYPDelayProjectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FYPDelayProjectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FYPDelayProjectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FYPDelayProjectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FYPDelayProjectAudioProcessor::getProgramName (int index)
{
    return {};
}

void FYPDelayProjectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FYPDelayProjectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    dsp::ProcessSpec spec;
    fSR = 44100;
    iBufferSize = (int)(2.0f * fSR);
    pfCircularBuffer = new float[iBufferSize];
    for (int x = 0; x<iBufferSize; x++)
        pfCircularBuffer[x] = 0;
    iBufferWritePos = 0;
    fDelayTime = 0.8;
    fFeedbackGain = 0.7;
    lowPassFilter[0]->prepare(spec);
    lowPassFilter[0]->reset();
    lowPassFilter[0]->setCutoffFrequency(8000);
}

void FYPDelayProjectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FYPDelayProjectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FYPDelayProjectAudioProcessor::updateParameters()
{
    fFeedbackGain = *apvts.getRawParameterValue("FEEDBACK");
    fDelayTime = *apvts.getRawParameterValue("TIME");
    saturation = *apvts.getRawParameterValue("SATURATION");
}

float FYPDelayProjectAudioProcessor::saturationTransfereFunction(float x)
{
    float y = 0.0f;
    float coeffA = 2.0f;
    if (x > 0.0f && x <= 1.0f)
    {
        y = (coeffA/(coeffA-1.0f))*(1.0f - pow(coeffA, -x));
    }
    else if (x <= 0.0f && x >= -1.0f)
    {
        y = (coeffA/(coeffA -1.0f))*(-1.0f + pow(coeffA, x));
    }
    return y;
}

void FYPDelayProjectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    auto* channelData = buffer.getWritePointer (0);

    for (int sample = 0; sample < buffer.getNumSamples(); sample++)
    {
        updateParameters();
        
        fDelSig = saturationTransfereFunction(fDelSig) + fDelSig;
        if (fDelSig > 1)
            fDelSig = 1;
        else if (fDelSig < -1)
            fDelSig = -1;
        
        fOut = fDelSig + channelData[sample];
        iBufferWritePos++;
        if (iBufferWritePos > (iBufferSize -1))
            iBufferWritePos = 0;
        pfCircularBuffer[iBufferWritePos] = fOut;
        iBufferReadPos = iBufferWritePos - (fDelayTime * fSR);
        if (iBufferReadPos < 0){
            iBufferReadPos = (iBufferSize - (fDelayTime * fSR)) + iBufferWritePos;
        }
        else
        {
            iBufferReadPos = iBufferWritePos - (fDelayTime * fSR);
        }
            
        if (iBufferReadPos > (iBufferSize -1 ))
            iBufferReadPos = 0;
        fDelSig = pfCircularBuffer[iBufferReadPos] * fFeedbackGain + (random.nextFloat() * 0.00025f - 0.000125f);
        channelData[sample] = fOut;
        buffer.copyFrom(1, 0, buffer, 0, 0, buffer.getNumSamples());
    }
}

//==============================================================================
bool FYPDelayProjectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FYPDelayProjectAudioProcessor::createEditor()
{
    return new FYPDelayProjectAudioProcessorEditor (*this);
}

//==============================================================================
void FYPDelayProjectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FYPDelayProjectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FYPDelayProjectAudioProcessor();
}

AudioProcessorValueTreeState::ParameterLayout FYPDelayProjectAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> params;
    params.push_back(std::make_unique<AudioParameterFloat>("FEEDBACK", "Feedback", 0.0f, 0.7f, 0.2f));
    params.push_back(std::make_unique<AudioParameterFloat>("TIME", "Time", 0.0f, 0.8f, 0.5f));
    params.push_back(std::make_unique<AudioParameterFloat>("SATURATION", "Saturation", 2.0f, 1000.0f, 2.0f));
    /*
    params.push_back(std::make_unique<AudioParameterFloat>("FEEDBACK", "Feedback", 0.0f, 0.99f, 0.8f));
    params.push_back(std::make_unique<AudioParameterFloat>("VIBRATO", "Vibrato", 0.001f, 1.000f, 0.00f ));
    params.push_back(std::make_unique<AudioParameterFloat>("DEPTH" , "Depth", 0.1f, 1.0f, 0.8f));
    params.push_back(std::make_unique<AudioParameterFloat>("INPUT", "Input", 0.0f, .5f, 0.01f));
    params.push_back(std::make_unique<AudioParameterFloat>("OUTPUT", "Ouput", 0.0f, 1.0f, 0.01f));
     */
    
    
    return {params.begin(), params.end() };
}
