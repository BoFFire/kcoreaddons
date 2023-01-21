/*
    SPDX-FileCopyrightText: 2013 Sebastian Kügler <sebas@kde.org>
    SPDX-FileCopyrightText: 2014 Alex Merry <alexmerry@kde.org>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/

#include "versionedplugin.h"
#include "kcoreaddons_debug.h"
#include <QDebug>
#include <kpluginfactory.h>

VersionedPlugin::VersionedPlugin(QObject *parent, const QVariantList &args)
    : QObject(parent)
{
    qCDebug(KCOREADDONS_DEBUG) << "Created VersionedPlugin with args" << args;
}

K_PLUGIN_FACTORY(VersionedPluginFactory, registerPlugin<VersionedPlugin>();)

#include "versionedplugin.moc"
