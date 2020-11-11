#pragma once

#include <JuceHeader.h>

struct DualButton : public Component
{
    DualButton();
    void resized() override;
    
private:
    TextButton button1{"button1"}, button2{"button2"};
};

//--------------------------------------------------------
struct RepeatingThing : Component, Timer
{
    
    void timerCallback() override
    {
        drawRed = !drawRed;
        repaint();
    }
    
    void paint (Graphics& g) override
    {
        g.fillAll(drawRed ? Colours::red : Colours::green);
    }
    
    RepeatingThing() {startTimerHz(2); }
    ~RepeatingThing() { stopTimer(); }
    
private:
    bool drawRed = true;
    
};
//--------------------------------------------------------
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

struct OwnedArrayComponent : Component, Button::Listener
{
    OwnedArrayComponent();
    ~OwnedArrayComponent();
    void resized() override;
    void buttonClicked(Button* buttonThatWasClicked) override;
    
private:
    OwnedArray<TextButton> buttons;
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
//        DBG("MainComponent mouseEnter " << counter);
//        ++counter;
    }
    void mouseExit(const MouseEvent& e) override
    {
//        DBG("MainComponent mouseExit " << counter);
//        ++counter;
    }
    void mouseMove(const MouseEvent& e) override
    {
//        DBG("MainComponent mouseMove " << counter);
//        ++counter;
    }
    void mouseDown(const MouseEvent& e) override
    {
        DBG("U done clicked") ;
    }
    

private:
    //int counter = 0;
    MyComp comp;
    OwnedArrayComponent ownedArrayComp;
    DualButton dualButton;
    
    RepeatingThing repeatingThing;
    
    //==============================================================================
    // Your private member variables go here...


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
