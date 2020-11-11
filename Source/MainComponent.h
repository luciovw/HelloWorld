#pragma once

#include <JuceHeader.h>

struct MyComp : Component
{
    void resized() override { }
    void paint(Graphics& g) override { g.fillAll(Colours::green); }
    
    void mouseEnter(const MouseEvent& e) override
    {
        DBG(" MyComp mouseEnter " << counter);
        ++counter;
    }
    void mouseExit(const MouseEvent& e) override
    {
        DBG("MyComp mouseExit " << counter);
        ++counter;
    }
    void mouseMove(const MouseEvent& e) override
    {
        DBG("MyComp mouseMove " << counter);
        ++counter;
    }
    
private:
    int counter = 0;
    
};

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void mouseEnter(const MouseEvent& e) override
    {
        DBG("MainComponent mouseEnter " << counter);
        ++counter;
    }
    void mouseExit(const MouseEvent& e) override
    {
        DBG("MainComponent mouseExit " << counter);
        ++counter;
    }
    void mouseMove(const MouseEvent& e) override
    {
        DBG("MainComponent mouseMove " << counter);
        ++counter;
    }
    

private:
    int counter = 0;
    MyComp comp;
    //==============================================================================
    // Your private member variables go here...


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
