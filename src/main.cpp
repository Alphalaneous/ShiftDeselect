#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/CCEGLView.hpp>

using namespace geode::prelude;

bool g_isNonModifierPressed = false;

class $modify(MyCCEGLView, CCEGLView) {

	void onGLFWKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

		if (key != GLFW_KEY_LEFT_CONTROL 
			&& key != GLFW_KEY_RIGHT_CONTROL 
			&& key != GLFW_KEY_LEFT_ALT 
			&& key != GLFW_KEY_RIGHT_ALT 
			&& key != GLFW_KEY_LEFT_SHIFT 
			&& key != GLFW_KEY_RIGHT_SHIFT) {
			g_isNonModifierPressed = (action == 1 || action == 2);
		}

		return CCEGLView::onGLFWKeyCallback(window, key, scancode, action, mods);
	}

	bool isNonModifierPressed() {
		return g_isNonModifierPressed;
	}
};

class $modify(EditorUI) {

	void deselectSpecificObject() {
		CCNode* batchLayer = m_editorLayer->getChildByIDRecursive("batch-layer");
		if(!batchLayer) return;

		CCPoint pos = getMousePos();
		CCPoint mousePosToNode = batchLayer->convertToNodeSpace(pos);

		for (GameObject* obj : geode::cocos::CCArrayExt<GameObject*>(m_selectedObjects)) {
			CCPoint objPos = obj->getPosition();
			CCSize objSize = obj->getScaledContentSize();
			CCRect objRect = {objPos.x - std::abs(objSize.width)/2, objPos.y - std::abs(objSize.height)/2, std::abs(objSize.width), std::abs(objSize.height)};

			LevelEditorLayer* levelEditorLayer = this->m_editorLayer;
			int currentLayer = levelEditorLayer->m_currentLayer;

			bool isOnCurrentEditorLayer1 = obj->m_editorLayer == levelEditorLayer->m_currentLayer;
			bool isOnCurrentEditorLayer2 = (obj->m_editorLayer2 == levelEditorLayer->m_currentLayer) && obj->m_editorLayer2 != 0;

			//theoretically works, but members are wrong

			if(objRect.containsPoint(mousePosToNode) && (currentLayer == -1 || (isOnCurrentEditorLayer1 || isOnCurrentEditorLayer2))) {
				deselectObject(obj);
				break;
			}
		}
	}

	void selectObject(GameObject* p0, bool p1) {
		if (!getKeyPressed()) {
			EditorUI::selectObject(p0, p1);
		}
	}

    void selectObjects(cocos2d::CCArray* p0, bool p1) {
		if (!getKeyPressed()) {
			EditorUI::selectObjects(p0, p1);
		}
		else{
			for (GameObject* obj : geode::cocos::CCArrayExt<GameObject*>(p0)) {
				deselectObject(obj);
			}
		}
	}

    void ccTouchEnded(cocos2d::CCTouch* p0, cocos2d::CCEvent* p1) {
		if(m_selectedMode == 3) {
			if (getKeyPressed()) {
				deselectSpecificObject();
			}
		}
		EditorUI::ccTouchEnded(p0, p1);
	}

	bool getKeyPressed(){
		auto kb = CCDirector::sharedDirector()->getKeyboardDispatcher();

		MyCCEGLView* eglView = static_cast<MyCCEGLView*>(CCEGLView::get());
		if (eglView->isNonModifierPressed()) return false;

		std::string setting = Mod::get()->getSettingValue<std::string>("modifier-key");

		if(setting == "Shift"){
			return kb->getShiftKeyPressed();
		}
		if(setting == "Alt"){
			return kb->getAltKeyPressed();
		}
		if(setting == "Ctrl"){
			return kb->getControlKeyPressed();
		}

		return false;
	}
};
