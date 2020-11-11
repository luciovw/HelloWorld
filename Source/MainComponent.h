#pragma once
#include <JuceHeader.h>
//struct RepeatingThing;

struct MyThread : Thread
{
    MyThread() : Thread("MyThread")
    {
        startThread();
    }
    ~MyThread()
    {
        stopThread(100);
    }
    
    void run() override
    {
        //threadShouldExit();
        while ( true )
        {
            if( threadShouldExit() )
                break;
            /*
             Do stuff
            */
            if( threadShouldExit() )
                break;
            
            /*
             Do some more stuff
             */
            if( threadShouldExit() )
                break;
            
            wait(10);
            //or
            wait(-1);
            /*
             MyThread mt;
             mt.notify();
             */
        }
    }
};

//==================================================================
struct ImageProcessingThread : Thread
{
    
    ImageProcessingThread(int w_, int h_);

    ~ImageProcessingThread();
    
    void run() override;
    
    void setUpdateRendererFunc(std::function<void(Image&&)> f);
    
private:
    int w {0};
    int h {0};
    Random r;
    
    std::function<void(Image&&)> updateRenderer;
};

//==================================================================
struct LambdaTimer : Timer
{
    LambdaTimer(int ms, std::function<void()> f);
    ~LambdaTimer();
    void timerCallback() override;
    
private:
    std::function<void()> lambda;
};

//==================================================================
#include<array>
struct Renderer : Component, AsyncUpdater
{
    Renderer();
    ~Renderer();
    
    void paint(Graphics& g ) override;
    void handleAsyncUpdate() override;
    
private:
    std::unique_ptr<ImageProcessingThread> processingThread;
    std::unique_ptr<LambdaTimer> lambdaTimer;
    
    bool firstImage = true;
    
    std::array<Image, 2> imageToRender;
    
    
};
//==================================================================

struct DualButton : public Component
{
    DualButton();
    void resized() override;
    
    void setButton1Handler(std::function<void()> f);
    void setButton2Handler(std::function<void()> f);
private:
    //RepeatingThing& timerThing;
    TextButton button1{"button1"}, button2{"button2"};
};

struct MyAsyncHighResGui : Component, AsyncUpdater, HighResolutionTimer
{
    void handleAsyncUpdate() override
    {
        paintColor = (paintColor + 1) % maxColors;
        repaint();
    }
    
    void hiResTimerCallback() override
    {
        triggerAsyncUpdate();
    }
    
    void paint (Graphics& g) override
    {
        switch (paintColor) {
            case 0:
                g.setColour(Colours::red);
                break;
            case 1:
                g.setColour(Colours::green);
                break;
            case 2:
                g.setColour(Colours::blue);
                break;
        }
        g.fillAll();
        
    }
    
    MyAsyncHighResGui()
    {
        this->startTimer(1000 / 5);
    }
    
    ~MyAsyncHighResGui()
    {
        stopTimer();
        cancelPendingUpdate();
    }
    
private:
    int paintColor = 0;
    const int maxColors {3};
    
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
    RepeatingThing repeatingThing;
    
    DualButton dualButton; //{repeatingThing};
    
    MyAsyncHighResGui hiResGui;
    
    Renderer renderer;
    
    //==============================================================================
    // Your private member variables go here...


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
