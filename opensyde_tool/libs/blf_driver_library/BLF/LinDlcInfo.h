/*
 * Copyright (C) 2013 Tobias Lorenz.
 * Contact: tobias.lorenz@gmx.net
 *
 * This file is part of Tobias Lorenz's Toolkit.
 *
 * Commercial License Usage
 * Licensees holding valid commercial licenses may use this file in
 * accordance with the commercial license agreement provided with the
 * Software or, alternatively, in accordance with the terms contained in
 * a written agreement between you and Tobias Lorenz.
 *
 * GNU General Public License 3.0 Usage
 * Alternatively, this file may be used under the terms of the GNU
 * General Public License version 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU General Public License version 3.0 requirements will be
 * met: http://www.gnu.org/copyleft/gpl.html.
 */

#pragma once

#include "platform.h"

#include <array>

#include "AbstractFile.h"
#include "ObjectHeader.h"
#include "VectorTypes.h"

#include "vector_blf_export.h"

namespace Vector {
namespace BLF {

/**
 * @brief LIN_DLC_INFO
 *
 * This info event occurs when the LIN hardware successfully detected the frame length
 * of an unknown frame. This frame length value is set as the one to be expected for this frame in the
 * future.
 */
class VECTOR_BLF_EXPORT LinDlcInfo : public ObjectHeader
{
public:
    LinDlcInfo();

    virtual void read(AbstractFile & is);
    virtual void write(AbstractFile & os);
    virtual size_t calculateObjectSize();

    /**
     * @brief application channel
     *
     * Channel number where the frame sent/received.
     */
    WORD channel;

    /**
     * @brief LIN ID
     *
     * Frame identifier
     */
    BYTE id;

    /**
     * @brief LIN DLC
     *
     * Frame length
     */
    BYTE dlc;

    /** reserved */
    std::array<BYTE, 4> reserved;
};

}
}
