#ifndef SIDE_H_
#define SIDE_H_

#include <iostream>

enum class Side {
    left,
    right,
    dnleft,
    dnright,
    upleft,
    upright,
    center,
    lipfunneldnleft,
    lipfunneldnright,
    lipfunnelupleft,
    lipfunnelupright,
    liptightendnleft,
    liptightendnright,
    liptightenupleft,
    liptightenupright
};

inline Side sideFromString(const std::string& str) {
    if (str == "left") return Side::left;
    else if (str == "right") return Side::right;
    else if (str == "dnleft") return Side::dnleft;
    else if (str == "dnright") return Side::dnright;
    else if (str == "upleft") return Side::upleft;
    else if (str == "upright") return Side::upright;
    else if (str == "center") return Side::center;
    else if (str == "lipfunneldnleft") return Side::lipfunneldnleft;
    else if (str == "lipfunneldnright") return Side::lipfunneldnright;
    else if (str == "lipfunnelupleft") return Side::lipfunnelupleft;
    else if (str == "lipfunnelupright") return Side::lipfunnelupright;
    else if (str == "liptightendnleft") return Side::liptightendnleft;
    else if (str == "liptightendnright") return Side::liptightendnright;
    else if (str == "liptightenupleft") return Side::liptightenupleft;
    else if (str == "liptightenupright") return Side::liptightenupright;
    return Side::center;
}

inline std::string sideToString(Side side) {
    switch (side) {
        case Side::left: return "left";
        case Side::right: return "right";
        case Side::dnleft: return "dnleft";
        case Side::dnright: return "dnright";
        case Side::upleft: return "upleft";
        case Side::upright: return "upright";
        case Side::center: return "center";
        case Side::lipfunneldnleft: return "lipfunneldnleft";
        case Side::lipfunneldnright: return "lipfunneldnright";
        case Side::lipfunnelupleft: return "lipfunnelupleft";
        case Side::lipfunnelupright: return "lipfunnelupright";
        case Side::liptightendnleft: return "liptightendnleft";
        case Side::liptightendnright: return "liptightendnright";
        case Side::liptightenupleft: return "liptightenupleft";
        case Side::liptightenupright: return "liptightenupright";
        default: return "unknown";
    }
}
#endif 