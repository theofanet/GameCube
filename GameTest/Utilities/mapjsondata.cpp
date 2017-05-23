#include "mapjsondata.h"

MapJsonData::MapJsonData(b2World *world){
    _world = world;
    _map = NULL;
}

void MapJsonData::fromJsonValue(Json::Value data){
    delete_obj(_map);
    
    _map = new Map(data["name"].asCString(), data["size"]["x"].asInt(), data["size"]["y"].asInt(), _world);
    
    if(data.isMember("sprites")){
        int i = 0;
        Json::Value sdata = data["sprites"][i++];
        while ( !sdata.isNull() ) {
            string spritefilename = _filename + "/" + sdata["spriteFileName"].asCString();
            _map->addTileset(spritefilename, sdata["spriteRowWidth"].asDouble(), sdata["spriteRowHeight"].asDouble());
            sdata = data["sprites"][i++];
        }
    }
    else{
        string spritefilename = _filename + "/" + data["spriteFileName"].asCString();
        _map->addTileset(spritefilename, data["spriteRowWidth"].asDouble(), data["spriteRowHeight"].asDouble());
    }
    
    if(data.isMember("start"))
        _map->setStartPosition(b2Vec2(data["start"]["x"].asDouble(), data["start"]["y"].asDouble()));
    
    if(data.isMember("end"))
        _map->setEndPosition(b2Vec2(data["end"]["x"].asDouble(), data["end"]["y"].asDouble()));
    
    for(int layer =0; layer < 3; layer++){
        int i = 0;
        Json::Value tdata = data["tiles"][layer][i++];
        while ( !tdata.isNull() ) {
            int index = 0;
            if(tdata.isMember("index"))
                index = tdata["index"].asInt();
            
            MapTile *t = new MapTile(_map->getTileset(index), _world);
            
            t->setPosition(tdata["position"]["x"].asDouble(), tdata["position"]["y"].asDouble());
            t->setSpritePosition(tdata["tileRow"].asInt(), tdata["tileCell"].asInt());
            if(tdata.isMember("is_destructible"))
                t->setDestructible(tdata["is_destructible"].asBool());
            if(tdata.isMember("is_climbable"))
                t->setClimbable(tdata["is_climbable"].asBool());
            if(tdata.isMember("is_slippery"))
                t->setSlippery(tdata["is_slippery"].asBool());
            if(tdata.isMember("is_deadly"))
                t->setDeadly(tdata["is_deadly"].asBool());
            if(tdata.isMember("horizontalMove"))
                t->setHorizontalMove(tdata["horizontalMove"].asBool(), tdata["horizontalMoveSpeed"].asDouble(), tdata["horizontalMoveMax"].asDouble());
            
            t->clearPoints();
            
            int j = 0;
            Json::Value point = tdata["points"][j++];
            while(!point.isNull()){
                t->addPoint(point["x"].asDouble(), point["y"].asDouble());
                point = tdata["points"][j++];
            }
            
            if(layer == 1)
                t->setBody();
            
            _map->addTile(t, layer);
            
            tdata = data["tiles"][layer][i++];
        }
    }
    
    if(data.isMember("checkpoints")){
        int i = 0;
        Json::Value tdata = data["checkpoints"][i++];
        while ( !tdata.isNull() ) {
            CheckPoint *t = new CheckPoint(_world);
            t->setPosition(b2Vec2(tdata["position"]["x"].asDouble(), tdata["position"]["y"].asDouble()));
            _map->addCheckpoint(t);
            tdata = data["checkpoints"][i++];
        }
    }
    
    if(data.isMember("objects")){
        int i = 0;
        
        vector<pair<Switch*, vector<pair<string, string>>>>  switchConnexions;
        vector<pair<PushButton*, vector<pair<string, string>>>>  pushButtonConnexions;
        vector<pair<AndDoor*, vector<pair<string, string>>>> andDoorConnexions;
        vector<pair<OrDoor*, vector<pair<string, string>>>>  orDoorConnexions;
        vector<pair<NoDoor*, vector<pair<string, string>>>>  noDoorConnexions;
        Switch  *s;
        PushButton *b;
        AndDoor *ad;
        OrDoor  *od;
        NoDoor  *nd;
        
        string basefilepath = _filename + "/";
        
        Json::Value tdata = data["objects"][i++];
        while(!tdata.isNull() ) {
            if(tdata.isMember("type")){
                Object *obj = NULL;
                
                switch(tdata["type"].asInt()){
                    case Object::OBJECT_TYPE_LASER:
                        obj = Laser::fromJsonData(_world, tdata, _map->getNbObjectOfType(Object::OBJECT_TYPE_LASER), basefilepath);
                        break;
                    case Object::OBJECT_TYPE_JETON:
                        obj = Jeton::fromJsonData(_world, tdata, _map->getNbObjectOfType(Object::OBJECT_TYPE_JETON), basefilepath);
                        break;
                    case Object::OBJECT_TYPE_EMITTER:
                        obj = ParticleEmitter::fromJsonData(_world, tdata, _map->getNbObjectOfType(Object::OBJECT_TYPE_EMITTER), basefilepath);
                        break;
                    case Object::OBJECT_TYPE_PLATFORM:
                        obj = Platform::fromJsonData(_world, tdata, _map->getNbObjectOfType(Object::OBJECT_TYPE_PLATFORM), basefilepath);
                        break;
                    case Object::OBJECT_TYPE_SWITCH:
                        s = Switch::fromJsonData(_world, tdata, _map->getNbObjectOfType(Object::OBJECT_TYPE_SWITCH), basefilepath);
                        switchConnexions.push_back(make_pair(s, s->getConnexionsToCreate()));
                        obj = s;
                        break;
                    case Object::OBJECT_TYPE_PUSH_BUTTON:
                        b = PushButton::fromJsonData(_world, tdata, _map->getNbObjectOfType(Object::OBJECT_TYPE_PUSH_BUTTON), basefilepath);
                        pushButtonConnexions.push_back(make_pair(b, b->getConnexionsToCreate()));
                        obj = b;
                        break;
                    case Object::OBJECT_TYPE_OR_DOOR:
                        od = OrDoor::fromJsonData(_world, tdata, _map->getNbObjectOfType(Object::OBJECT_TYPE_OR_DOOR), basefilepath);
                        orDoorConnexions.push_back(make_pair(od, od->getConnexionsToCreate()));
                        obj = od;
                        break;
                    case Object::OBJECT_TYPE_NO_DOOR:
                        nd = NoDoor::fromJsonData(_world, tdata, _map->getNbObjectOfType(Object::OBJECT_TYPE_NO_DOOR), basefilepath);
                        noDoorConnexions.push_back(make_pair(nd, nd->getConnexionsToCreate()));
                        obj = nd;
                    case Object::OBJECT_TYPE_AND_DOOR:
                        ad = AndDoor::fromJsonData(_world, tdata, _map->getNbObjectOfType(Object::OBJECT_TYPE_AND_DOOR), basefilepath);
                        andDoorConnexions.push_back(make_pair(ad, ad->getConnexionsToCreate()));
                        obj = ad;
                        break;
                    default:
                        break;
                }
                
                if(obj)
                    _map->addObject(obj);
                tdata = data["objects"][i++];
                
                for(vector<pair<Switch*, vector<pair<string, string>>>>::iterator it = switchConnexions.begin(); it != switchConnexions.end(); ++it){
                    for(vector<pair<string, string>>::iterator iter = it->second.begin(); iter != it->second.end(); ++iter){
                        Object *o = _map->getObject(iter->first);
                        if(o)
                            it->first->connect(o, iter->second);
                    }
                }
                
                for(vector<pair<PushButton*, vector<pair<string, string>>>>::iterator it = pushButtonConnexions.begin(); it != pushButtonConnexions.end(); ++it){
                    for(vector<pair<string, string>>::iterator iter = it->second.begin(); iter != it->second.end(); ++iter){
                        Object *o = _map->getObject(iter->first);
                        if(o)
                            it->first->connect(o, iter->second);
                    }
                }
                
                for(vector<pair<OrDoor*, vector<pair<string, string>>>>::iterator it = orDoorConnexions.begin(); it != orDoorConnexions.end(); ++it){
                    for(vector<pair<string, string>>::iterator iter = it->second.begin(); iter != it->second.end(); ++iter){
                        Object *o = _map->getObject(iter->first);
                        if(o)
                            it->first->connect(o, iter->second);
                    }
                }
                
                for(vector<pair<NoDoor*, vector<pair<string, string>>>>::iterator it = noDoorConnexions.begin(); it != noDoorConnexions.end(); ++it){
                    for(vector<pair<string, string>>::iterator iter = it->second.begin(); iter != it->second.end(); ++iter){
                        Object *o = _map->getObject(iter->first);
                        if(o)
                            it->first->connect(o, iter->second);
                    }
                }
                
                for(vector<pair<AndDoor*, vector<pair<string, string>>>>::iterator it = andDoorConnexions.begin(); it != andDoorConnexions.end(); ++it){
                    for(vector<pair<string, string>>::iterator iter = it->second.begin(); iter != it->second.end(); ++iter){
                        Object *o = _map->getObject(iter->first);
                        if(o)
                            it->first->connect(o, iter->second);
                    }
                }
            }
        }
    }
    else{
        if(data.isMember("jetons")){
            int i = 0;
            Json::Value tdata = data["jetons"][i++];
            while ( !tdata.isNull() ) {
                Jeton *t = Jeton::fromJsonData(_world, tdata, _map->getNbObjectOfType(Object::OBJECT_TYPE_JETON), "");
                _map->addObject(t);
                tdata = data["jetons"][i++];
            }
        }
        
        if(data.isMember("lasers")){
            int i = 0;
            Json::Value tdata = data["lasers"][i++];
            while ( !tdata.isNull() ) {
                Laser *t = Laser::fromJsonData(_world, tdata, _map->getNbObjectOfType(Object::OBJECT_TYPE_LASER), "");
                _map->addObject(t);
                tdata = data["lasers"][i++];
            }
        }
        
        if(data.isMember("emitters")){
            int i = 0;
            Json::Value tdata = data["emitters"][i++];
            while ( !tdata.isNull() ) {
                ParticleEmitter *t = ParticleEmitter::fromJsonData(_world, tdata, _map->getNbObjectOfType(Object::OBJECT_TYPE_EMITTER), "");
                _map->addObject(t);
                tdata = data["emitters"][i++];
            }
        }
    }
}

void MapJsonData::saveToFile(string filename){
    Json::Value data = toJsonValue();
    
    std::ofstream ofs;
    ofs.open(filename.c_str(), std::ios::out);
    if (!ofs) {
        std::cout << "Could not open file " << filename << " for writing\n";
        return;
    }
    
    Json::StyledStreamWriter writer("   ");
    writer.write( ofs, data );
    
    ofs.close();
}

void MapJsonData::loadFromFile(string filename){
    string data_filename = "../" + filename + "/data.mjs";
    
    std::ifstream ifs;
    ifs.open(data_filename.c_str(), std::ios::in);
    if (!ifs) {
        std::cout << "Could not open file '" << data_filename << "' for reading" << std::endl;
        return;
    }
    
    Json::Value  data;
    Json::Reader reader;
    if ( ! reader.parse(ifs, data) ) {
        std::cout << "Failed to parse '" << data_filename << "' : " << reader.getFormatedErrorMessages() << endl;
        ifs.close();
        return;
    }
    ifs.close();
    
    _filename = filename;
    
    fromJsonValue(data);
}