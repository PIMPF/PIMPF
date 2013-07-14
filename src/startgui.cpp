//  Copyright (c) 2012-2013 Thomas Heller
//  Copyright (c) 2012-2013 Andreas Schaefer
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "startgui.hpp"
#include "particlewidget.hpp"

#include <hpx/hpx.hpp>

#include <QApplication>

namespace {
    void runWidget(
        boost::shared_ptr<hpx::lcos::local::promise<void> > finishedPromise)
    {
        QCoreApplication *app = QApplication::instance();
        vandouken::ParticleWidget *flow = new vandouken::ParticleWidget();

        flow->resize(1000, 500);
        flow->show();

        app->exec();
        finishedPromise->set_value();
    }
}

namespace vanDouken
{
    void startGUI(boost::program_options::variables_map& vm)
    {
        boost::shared_ptr<hpx::lcos::local::promise<void> > finishedPromise;
        hpx::future<void> finished(finishedPromise->get_future());

        hpx::util::io_service_pool *main_pool =
            hpx::get_runtime().get_thread_pool("main_pool");

        main_pool->get_io_service().post(
            HPX_STD_BIND(
                &::runWidget
              , finishedPromise
            )
        );

        hpx::wait(finished);
    }
}