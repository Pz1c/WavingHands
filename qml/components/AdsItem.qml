import QtQuick 2.9
import QtFirebase 1.0

Item {
    property alias ready: amiFullScreen.ready
    property alias loaded: amiFullScreen.loaded
    property int l_interstitial_type: 0
    property double l_interstitial_show: 0
    property int l_interstitial_show_value: 5

    AdMob {
        //appId: Qt.platform.os == "android" ? "ca-app-pub-6606648560678905~6485875670" : "ca-app-pub-6606648560678905~1693919273"
        appId: "ca-app-pub-8386996307721393~9694142607"

        // NOTE All banners and interstitials will use this list
        // unless they have their own testDevices list specified
        testDevices: [
            "E348963E5524891CF469D7B33E9B959B"
        ]
    }

    AdMobInterstitial {
        id: amiFullScreen
        //adUnitId: Qt.platform.os == "android" ? "ca-app-pub-6606648560678905/3118450073" : "ca-app-pub-6606648560678905/7548649672"
        adUnitId: "ca-app-pub-8386996307721393/7610904746"; // Android
        //adUnitId: "ca-app-pub-6606648560678905/7548649672"; // iOS

        onReadyChanged: {
            console.log("amiFullScreen", "onReadyChanged", ready, loaded);
            /*if(ready) {
                load();
            }*/
        }

        onLoadedChanged: {
            console.log("amiFullScreen", "onLoadedChanged", ready, loaded);
            //if(loaded) { show(); }
        }

        onClosed: {
            console.log("amiFullScreen", "onClosed", ready, loaded);
            load();
            switch(l_interstitial_type) {
            case 0:
                // nothing to do
                break;
            case 1:
                // restart level
                //startGame(0, Qt.GameStats.CurrentLevel);
                break;
            }
        }

        onError: {
            console.log("Interstitial failed with error code",code,"and message",message)
            // See AdMob.Error* enums
            if(code === AdMob.ErrorNetworkError) {
                console.log("No network available");
            }
        }

        request: AdMobRequest {
            gender: AdMob.GenderUnknown
            childDirectedTreatment: AdMob.ChildDirectedTreatmentUnknown
        }
    }

    function showInterstitial() {
        //console.log("showInterstitial", amiFullScreen.ready, l_interstitial_show);
        if (amiFullScreen.ready && amiFullScreen.loaded && (l_interstitial_show >= l_interstitial_show_value)) {
            l_interstitial_show -= l_interstitial_show_value;
            amiFullScreen.show();
            return true;
        }
        return false;
    }

    function addInterstitialShow(val) {
        console.log("addInterstitialShow", l_interstitial_show, val, l_interstitial_show_value);
        l_interstitial_show += val;
        if (l_interstitial_show >= l_interstitial_show_value * 2) {
            l_interstitial_show = l_interstitial_show_value * 2;
        }
    }
}
