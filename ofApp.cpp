#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
	ofNoFill();

	this->theta_seed = ofRandom(1000);
	this->scale_seed = ofRandom(1000);

	ofColor color;
	vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	for (auto hex : hex_list) {

		color.setHex(hex);
		this->base_color_list.push_back(color);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	while (this->param_list.size() < 100) {

		auto deg_pair = make_pair(ofRandom(360), ofRandom(3, 20));
		this->param_list.push_back(deg_pair);
		vector<glm::vec2> log;
		this->log_list.push_back(log);
		this->color_list.push_back(this->base_color_list[(int)ofRandom(this->base_color_list.size())]);
		this->life_list.push_back(ofRandom(60, 180));
	}

	for (int i = this->param_list.size() - 1; i >= 0; i--) {

		this->life_list[i] -= 1;
		if (this->life_list[i] < 0) {

			this->param_list.erase(this->param_list.begin() + i);
			this->log_list.erase(this->log_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
			this->life_list.erase(this->life_list.begin() + i);

			continue;
		}

		auto noise_location = this->make_point(this->param_list[i].first);
		auto u_param = ofMap(ofNoise(glm::vec4(noise_location * 0.003, this->param_list[i].second * 0.03, this->theta_seed + ofGetFrameNum() * 0.003)), 0, 1, -0.5, 0.5);
		auto v_param = ofMap(ofNoise(glm::vec4(noise_location * 0.003, this->param_list[i].second * 0.03, this->scale_seed + ofGetFrameNum() * 0.008)), 0, 1, 3, 20);

		this->param_list[i].first += u_param;
		this->param_list[i].second = v_param;

		this->log_list[i].push_back(this->make_point(this->param_list[i].first) * this->param_list[i].second);
		while (this->log_list[i].size() > 100) {

			this->log_list[i].erase(this->log_list[i].begin());
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	for (int i = 0; i < this->param_list.size(); i++) {

		ofSetColor(this->color_list[i]);

		if (this->life_list[i] > 60) {

			ofSetLineWidth(1.5);
		}
		else {

			ofSetLineWidth(ofMap(this->life_list[i], 0, 60, 0, 1.5));
		}

		ofBeginShape();
		ofVertices(this->log_list[i]);
		ofEndShape();
	}
}

//--------------------------------------------------------------
// Reference by https://twitter.com/shiffman/status/1095764239665512453
glm::vec2 ofApp::make_point(float theta) {

	float x = 16 * (pow(sin(theta), 3));
	float y = 13 * cos(theta) - 5 * cos(2 * theta) - 2 * cos(3 * theta) - cos(4 * theta);
	return glm::vec2(x, -y);
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}