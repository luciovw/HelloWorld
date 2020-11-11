#include "MainComponent.h"

//============================================================

ImageProcessingThread::ImageProcessingThread(int w_, int h_) : Thread("ImageProcessingThread"), w(w_), h(h_)
{
    startThread();
}

ImageProcessingThread::~ImageProcessingThread()
{
    stopThread(500);
}

void ImageProcessingThread::run()
{
    while ( true )
    {
        if( threadShouldExit() )
            break;
        
        auto canvas = Image(Image::PixelFormat::RGB, w, h, true);
        
        if( threadShouldExit() )
            break;
        DBG("[ImageProcessingThread] generating random image: " << Time::getCurrentTime().toISO8601(true) << "\n" );
        
        bool shouldBail = false;
        
        for (int x = 0; x < w; ++x)
        {
            if( threadShouldExit() )
            {
                shouldBail = true;
                break;
            }
            for (int y = 0; y < h; ++y)
            {
                canvas.setPixelAt(x,
                                  y,
                                  Colour(r.nextFloat(),
                                         r.nextFloat(),
                                         r.nextFloat(),
                                         1.f )
                                  );
            }
        }
        
        if( threadShouldExit() || shouldBail )
            break;
        
        if( updateRenderer )
            updateRenderer( std::move(canvas) );
        
        wait(-1);
        /*
         MyThread mt;
         mt.notify();
         */
    }
}

void ImageProcessingThread::setUpdateRendererFunc(std::function<void(Image&&)> f)
{
    updateRenderer = std::move(f);
}
//============================================================

LambdaTimer::LambdaTimer(int ms, std::function<void()> f) : lambda(std::move(f) ) 
{
    startTimer(ms);
}

LambdaTimer::~LambdaTimer()
{
    stopTimer();
}

void LambdaTimer::timerCallback()
{
    stopTimer();
    
    if( lambda )
        lambda();
}
//============================================================

Renderer::Renderer()
{
    lambdaTimer = std::make_unique<LambdaTimer>(10, [this] ()
    {
        processingThread = std::make_unique<ImageProcessingThread>(getWidth(),
                                                                   getHeight());
        
        processingThread->setUpdateRendererFunc([this](Image&& image)
                                                
        {
            
            int renderIndex = firstImage ? 0 : 1;
            firstImage = !firstImage;
            imageToRender[renderIndex] = std::move(image);
            
            triggerAsyncUpdate();
            
            lambdaTimer = std::make_unique<LambdaTimer>(100, [this]()
            {
                processingThread->notify();
            });
        });
    });
}

Renderer::~Renderer()
{
    processingThread.reset();
    lambdaTimer.reset();
}

void Renderer::paint(Graphics& g )
{
    DBG("[Renderer] painting: " << Time::getCurrentTime().toISO8601(true) << "\n" );
    
    g.drawImage(firstImage ? imageToRender[0] : imageToRender[1], getLocalBounds().toFloat() );
}

void Renderer::handleAsyncUpdate()
{
    repaint();
}

//============================================================
DualButton::DualButton()
{
    addAndMakeVisible(button1);
    addAndMakeVisible(button2);
    
/*
    button1.onClick = [this] ()
    {
        DBG("Button1's size: " << this->button1.getBounds().toString() );
        timerThing.startTimerHz(2);
    };
    
    button2.onClick = [this] ()
    {
        DBG("Button2's size: " << this->button2.getBounds().toString() );
        timerThing.startTimerHz(4);
    };
 */
    
}

void DualButton::resized()
{
    auto bounds = getLocalBounds();
    button1.setBounds( bounds.removeFromLeft(30) );
    button2.setBounds( bounds );
}

void DualButton::setButton1Handler(std::function<void ()> f)
{
    button1.onClick = f;
}

void DualButton::setButton2Handler(std::function<void ()> f)
{
    button2.onClick = f;
}

//============================================================

//============================================================
OwnedArrayComponent::OwnedArrayComponent()
{
    for (int i = 0; i < 10; i++)
    {
        auto widget = buttons.add( new TextButton( String(i) ) );
        addAndMakeVisible(widget);
        widget -> addListener(this);
    }
}

OwnedArrayComponent::~OwnedArrayComponent()
{
    for (auto* widget : buttons)
    {
        widget -> removeListener(this);
    }
}

void OwnedArrayComponent::resized()
{
    auto width = getWidth() / static_cast<float>(buttons.size() );
    int x = 0;
    auto h = getHeight();
    
    for (auto* widget : buttons)
    {
        widget -> setBounds(x, 0, width, h);
        x += width;
    }
}

void OwnedArrayComponent::buttonClicked(Button *buttonThatWasClicked)
{
    if(buttonThatWasClicked == buttons.getFirst() )
    {
        DBG("You clicked the FIRST button");
    }
    else if (buttonThatWasClicked == buttons.getLast() )
    {
        DBG("You clicked the LAST button");
    }
    else
    {
        DBG("You clicked the SOME OTHER button");
    }
}

//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible(comp);
    //comp.addMouseListener(this, false);
    
    addAndMakeVisible(ownedArrayComp);
    ownedArrayComp.addMouseListener(this, true);
    addAndMakeVisible(dualButton);
    
    dualButton.setButton1Handler([this] ()
    {
//        DBG("Button1's size: " << this->dualButton.button1.getBounds().toString() );
        repeatingThing.startTimerHz(2);
    });
    
    dualButton.setButton2Handler([this] ()
    {
//        DBG("Button1's size: " << this->dualButton.button1.getBounds().toString() );
    repeatingThing.startTimerHz(4);
    });
    
    addAndMakeVisible(repeatingThing);
    addAndMakeVisible(hiResGui);
    addAndMakeVisible(renderer);
    
    setSize (600, 400);
}

MainComponent::~MainComponent()
{
    comp.removeMouseListener(this);
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont (juce::Font (16.0f));
    g.setColour (juce::Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    comp.setBounds(30, 30, 100, 200);
    ownedArrayComp.setBounds(comp.getX(),
                             comp.getBottom() + 5,
                             getWidth() - comp.getX(),
                             getHeight() - comp.getBottom()
                             );
    dualButton.setBounds(comp.getBounds().withX( comp.getRight() + 5 ));
    
    repeatingThing.setBounds(dualButton.getBounds().withX( dualButton.getRight() +5 ) );
    
    hiResGui.setBounds(repeatingThing.getBounds().withX( repeatingThing.getRight() +5 ) );
    
    renderer.setBounds(hiResGui.getBounds().withX( hiResGui.getRight() +5 ) );
}
