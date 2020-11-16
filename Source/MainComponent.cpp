#include "MainComponent.h"

//============================================================

ImageProcessingThread::ImageProcessingThread(int w_, int h_, ImagePassingFunc f) : Thread("ImageProcessingThread"), w(w_), h(h_), updateRenderer( std::move(f))
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
        
        for (int x = 0; x < w; ++x)
        {
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
        
        if( updateRenderer )
            updateRenderer( canvas, *this );
        
        wait(-1);
        /*
         MyThread mt;
         mt.notify();
         */
    }
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
                                                                   getHeight(),
                                                                   [this](Image image, ImageProcessingThread& thread)
                                                
        {
            imageToRender.push(image);
            
            if ( !thread.threadShouldExit() )
            {
                
                lambdaTimer = std::make_unique<LambdaTimer>(1000, [this]()
                {
                    processingThread->notify();
                });
            }
            
        });
    });
    
    startTimerHz(20);
}

Renderer::~Renderer()
{
    lambdaTimer.reset();
    processingThread.reset();
}

void Renderer::paint(Graphics& g )
{
    g.drawImage( imageToRender.Read(), getLocalBounds().toFloat() );
}

void Renderer::timerCallback()
{
    repaint();
}

//============================================================

Renderer2::Renderer2()
{
    Timer::callAfterDelay(10, [this]()
    {
        SafePointer<Renderer2> safePtr(this);
        if ( safePtr.getComponent() )
            safePtr->loop();
    } );
}

void Renderer2::paint(Graphics& g)
{
    g.drawImage( imageToRender.Read(), getLocalBounds().toFloat() );
}

void Renderer2::loop()
{
    auto w = getWidth();
    auto h = getHeight();
    
    Thread::launch([w, h, this]()
    {
        Random r;
        
        auto canvas = Image(Image::PixelFormat::RGB, w, h, true);
        for (int x = 0; x < w; ++x)
        {
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
        
        SafePointer<Renderer2> safePtr(this);
        if ( safePtr.getComponent() )
            safePtr->imageToRender.push(canvas);
        
        Timer::callAfterDelay(10, [this]()
        {
            SafePointer<Renderer2> safePtr(this);
            if ( safePtr.getComponent() )
                safePtr->repaint();
        });
        
        Timer::callAfterDelay(1000, [this]()
        {
            SafePointer<Renderer2> safePtr(this);
            if ( safePtr.getComponent() )
                safePtr->loop();;
        });
        
    });
}

//============================================================

DualButton::DualButton()
{
    addAndMakeVisible(button1);
    addAndMakeVisible(button2);
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
    
    addAndMakeVisible(ownedArrayComp);
    ownedArrayComp.addMouseListener(this, true);
    addAndMakeVisible(dualButton);
    
    dualButton.setButton1Handler([this] ()
    {
        repeatingThing.startTimerHz(2);
    });
    
    dualButton.setButton2Handler([this] ()
    {
    repeatingThing.startTimerHz(4);
    });
    
    addAndMakeVisible(repeatingThing);
    addAndMakeVisible(hiResGui);
    addAndMakeVisible(renderer);
    addAndMakeVisible(renderer2);
    
    setSize (800, 400);
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
    
    renderer2.setBounds(renderer.getBounds().withX( renderer.getRight() +5 ) );
}
