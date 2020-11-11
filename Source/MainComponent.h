#pragma once

#include <JuceHeader.h>

struct Widget : public Component
{
    Widget(int i) : num(i) {}
    void paint(Graphics& g) override
    {
        g.fillAll(Colours::red);
        g.setColour(Colours::black);
        g.drawRect( getLocalBounds().reduced(2) );
        
        g.drawFittedText(String(num),
                         getLocalBounds(),
                         Justification::centred,
                         1);
    }
    int num = 0;
    
};

struct OwnedArrayComponent : Component
{
    OwnedArrayComponent()
    {
        for (int i = 0; i < 10; i++)
        {
            auto widget = widgets.add( new Widget(i) );
            addAndMakeVisible(widget);
        }
    }
    
    void resized() override
    {
        auto width = getWidth() / static_cast<float>(widgets.size() );
        int x = 0;
        auto h = getHeight();
        
        for (auto* widget : widgets)
        {
            widget -> setBounds(x, 0, width, h);
            x += width;
        }
    }
    
    OwnedArray<Widget> widgets;
};

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
    OwnedArrayComponent ownedArrayComp;
    //==============================================================================
    // Your private member variables go here...


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
