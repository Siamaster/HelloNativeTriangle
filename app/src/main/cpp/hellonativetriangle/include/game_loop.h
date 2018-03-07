#pragma once

#include "renderer.h"

#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>

using namespace std;

class GameLoop {

public:
    explicit GameLoop(unique_ptr<Renderer> renderer) : renderer_{move(renderer)} {}

    ~GameLoop();

    GameLoop(const GameLoop &) = delete;
    GameLoop &operator=(const GameLoop &) = delete;

    void Start();
    void Stop();

    void ChangeRenderer(unique_ptr<Renderer>);
private:
    void Loop();

    unique_ptr<Renderer> renderer_;

    enum class Message {
        kInitialize,
        kPlay,
        kPause,
        kStop
    } msg_ = Message::kInitialize;

    mutex mutex_{};
    condition_variable condition_var{};
    thread thread_{};

    bool started_once_ = false;

    static void thread_callback_(GameLoop *);
};