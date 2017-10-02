import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Page {
    SwipeView {
        id: swipeView
        anchors.fill: parent

        SignInPage {
            id: signInPage
            onShowSignupScreenRequested: {
                showSignupPage()
            }
        }

        SignupPage {
            id: signupPage
            onShowSignInScreenRequested: {
                showSignInPage()
            }
        }
    }

    function showSignInPage() {
        swipeView.setCurrentIndex(signInPage.SwipeView.index)
    }

    function showSignupPage() {
        swipeView.setCurrentIndex(signupPage.SwipeView.index)
    }
}
