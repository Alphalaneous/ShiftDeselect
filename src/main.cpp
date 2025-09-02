#include <Geode/Geode.hpp>
#include <Geode/modify/EditorUI.hpp>

using namespace geode::prelude;

class $modify(EditorUI) {

	void deselectSpecificObject() {
		CCNode* batchLayer = m_editorLayer->m_objectsLayer:

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
		auto kb = CCDirector::get()->getKeyboardDispatcher();

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
