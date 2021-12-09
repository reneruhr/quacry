#include "quasicrystals_guimathcontrol.h"

void DrawColumnMatrix4(const Mat4& m) 
{
    for(int j =0; j <4; j++){
        for (int i = 0; i < 4; i++){
            ImGui::Text("%.1f", m[i][j]);
            if(i<3) ImGui::SameLine();
        }
    }
}

void DrawMatrix5(const Mat5f& m) 
{
    for(int j =0; j <5; j++){
        for (int i = 0; i < 5; i++){
            ImGui::Text("%.1f", m(j,i));
            if(i<4) ImGui::SameLine();
        }
    }
}

void DrawRowMatrix4(const Mat4& m) 
{
    for (int i = 0; i < 4; i++){
        for(int j =0; j <4; j++){
            ImGui::Text("%.1f", m[i][j]);
            if(j<3) ImGui::SameLine();
        }
    }
}

void EmbeddingMap(std::array<int, 16> &selected, SL2Embedding &embedding)
{
    if(selected[0] && selected[1] && selected[4] && selected[5])
        embedding = TOP_LEFT;
    else if(selected[0] && selected[3] && selected[12] && selected[15])
        embedding = OUTER;
    else if(selected[10] && selected[11] && selected[14] && selected[15])
        embedding = BOTTOM_RIGHT;
    else if(selected[2] && selected[3] && selected[6] && selected[7])
        embedding = TOP_RIGHT;
    else if(selected[8] && selected[9] && selected[12] && selected[13])
        embedding = BOTTOM_LEFT;
    else if(selected[0] && selected[2] && selected[8] && selected[10])
        embedding = XZ;
    else if(selected[5] && selected[6] && selected[9] && selected[10])
        embedding = YZ;
    else if(selected[5] && selected[7] && selected[13] && selected[15])
        embedding = YW;
    else embedding = NOT_DEFINED;
}

auto MakeDescription() -> vector<EmbeddingDescription> 
{
    int tl_arr[4] = {0,1,4,5};
    int outer_arr[4] = {0,3,12,15};
    int br_arr[4] = {10,11,14,15};
    int tr_arr[4] = {2,3,6,7};
    int bl_arr[4] = {8,9,12,13};
    int xz_arr[4] = {0,2,8,10};
    int yz_arr[4] = {5,6,9,10};
    int yw_arr[4] = {5,7,13,15};
    int nd_arr[4] = {-1,-1,-1,-1};

    EmbeddingDescription tl(TOP_LEFT, tl_arr, "Top Left");
    EmbeddingDescription outer(OUTER, outer_arr, "Outer");
    EmbeddingDescription br(BOTTOM_RIGHT, br_arr, "Bottom Right");
    EmbeddingDescription tr(TOP_RIGHT, tr_arr, "Top Right");
    EmbeddingDescription bl(BOTTOM_LEFT, bl_arr, "Bottom Left");
    EmbeddingDescription xz(XZ, xz_arr, "x-z");
    EmbeddingDescription yz(YZ, yz_arr, "y-z");
    EmbeddingDescription yw(YW, yw_arr, "y-w");
    EmbeddingDescription nd(NOT_DEFINED, nd_arr, "???");

    return {tl, outer, br, tr, bl ,xz, yz, yw, nd};
}

void EmbeddingMapReverse(std::array<int, 16> &selected, SL2Embedding &embedding){
    static auto all_embeddings = MakeDescription();
    for(auto& emb: all_embeddings){
        if(emb.embed == embedding){
            for(int i =0; i<16; ++i){
                if(i == emb.pos[0] || i == emb.pos[1] || i == emb.pos[2] || i == emb.pos[3]) selected[i] = 1;
                    else selected[i] = 0;
            }
            break;
        }
    }
}

bool DrawSL2Matrix(float& s, SL2Type SL2Type, int id=1)
{
    switch(SL2Type){
        case(DILATE):
            ImGui::Text("Dilate");
            ImGui::Text("%.1f", exp(s));
            ImGui::SameLine();
            ImGui::Text("%.1f", 0.0);
            ImGui::Text("%.1f", 0.0);
            ImGui::SameLine();
            ImGui::Text("%.1f", exp(-s));
            if(ImGui::SliderFloat("##float_dilate", &s, -10.0, 10.0)) return true;
            break;

        case(SHEAR_U):
            ImGui::Text("Shear");
            ImGui::Text("%.1f", 1.0);
            ImGui::SameLine();
            ImGui::Text("%.1f", s);
            ImGui::Text("%.1f", 0.0);
            ImGui::SameLine();
            ImGui::Text("%.1f", 1.0);
            if(ImGui::SliderFloat("##float_shearU", &s, -10.0, 10.0)) return true;
            break;

        case(SHEAR_L):
            ImGui::Text("Shear");
            ImGui::Text("%.1f", 1.0);
            ImGui::SameLine();
            ImGui::Text("%.1f", 0.0);
            ImGui::Text("%.1f", s);
            ImGui::SameLine();
            ImGui::Text("%.1f", 1.0);
            if(ImGui::SliderFloat("##float_shearL", &s, -10.0, 10.0)) return true;
            break;

        case(ROTATE):
            float angle = s * M_PI;
            ImGui::PushID(id);
            ImGui::Text("Rotate");
            ImGui::Text("%.1f", cos(angle));
            ImGui::SameLine();
            ImGui::Text("%.1f", -sin(angle));
            ImGui::Text("%.1f", sin(angle));
            ImGui::SameLine();
            ImGui::Text("%.1f", cos(angle));
            if(ImGui::SliderFloat("##float_rotate", &s, 0.0, 1.0)) {ImGui::PopID(); return true;}
            ImGui::PopID();
            break;
    }
    return false;
}

void EmbeddingsView(std::array<int, 16> &selected){
    ImGui::Separator();
    for (int i = 0; i < 4 * 4; i++){
        ImGui::PushID(i);
        if(selected[i]){
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
            ImGui::TextUnformatted("x");
            ImGui::PopStyleColor();
        }else ImGui::TextUnformatted("0");
        if ((i % 4) < 3) ImGui::SameLine();
        ImGui::PopID();
    }
}

void EmbeddingsView5(std::array<int, 16> &selected){
    ImGui::Separator();
    for (int i = 0; i < 4 * 4; i++){
        ImGui::PushID(i);
        if(selected[i]){
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
            ImGui::TextUnformatted("x");
            ImGui::PopStyleColor();
        }else ImGui::TextUnformatted("0");
        if ((i % 4) < 3) {
            ImGui::SameLine();
        }else {
            ImGui::SameLine();
            ImGui::TextUnformatted("0");
        }
        ImGui::PopID();
    }
    for(int i =0; i<5; i++) {
        ImGui::TextUnformatted("0");
        ImGui::SameLine();
    }
}

void DrawEmbeddings(std::array<int, 16> &selected, SL2Embedding &embedding)
{
    static int e = 0;
    ImGui::RadioButton("Top Left", &e, 0);
    ImGui::RadioButton("Outer", &e, 1);
    ImGui::RadioButton("Bottom Right", &e, 2);
    ImGui::RadioButton("Top Right", &e, 3);
    ImGui::SameLine();
    kipod::HoverInfo("*"," 'Unipotent Embbeding' is SL2-id");
    ImGui::NextColumn();
    ImGui::RadioButton("Bottom Left", &e, 4);
    ImGui::SameLine();
    kipod::HoverInfo("*"," 'Unipotent Embbeding' is SL2-id");
    ImGui::RadioButton("x-z", &e, 5);
    ImGui::RadioButton("y-z", &e, 6);
    ImGui::RadioButton("y-w", &e, 7);
    embedding = static_cast<SL2Embedding>(e);
    EmbeddingMapReverse(selected, embedding);
}

bool SL2Control(Mat4& current_transform, MatrixWalk &SL4walk, SL2Embedding embedding)
{
    bool changed = false;

    static float dilate = 0;
    static float shearU = 0;
    static float shearL = 0;
    static float rotate1 = 0;
    static float rotate2 = 0;
    static vector<SL2InSL4> five_identities = {
        std::make_tuple(ROTATE, embedding, rotate1),
        std::make_tuple(SHEAR_L, embedding, shearL),
        std::make_tuple(DILATE, embedding, dilate),
        std::make_tuple(SHEAR_U, embedding, shearU),
        std::make_tuple(ROTATE, embedding, rotate2)
    };
    static MatrixWalk local_five(five_identities);
    static Mat4 total = SL4::i; 

    static bool immediate_mode = true;
    ImGui::Text("SL2 Embedding:");
    ImGui::SameLine();
    ImGui::Checkbox("Immediate Mode", &immediate_mode);
    ImGui::SameLine();
    if(ImGui::Button("Reset")){
        changed = true;
        shearU=0; dilate =0; shearL = 0; rotate1 =0; rotate2=0;
        local_five.Modify(0, std::make_tuple(ROTATE, embedding, rotate1));
        local_five.Modify(1, std::make_tuple(SHEAR_L, embedding, shearL));
        local_five.Modify(2, std::make_tuple(DILATE, embedding, dilate));
        local_five.Modify(3, std::make_tuple(SHEAR_U, embedding, shearU));
        local_five.Modify(4, std::make_tuple(ROTATE, embedding, rotate2));
    }
    ImGui::SameLine();
    ImGui::Text("%s", EmbeddingName(embedding));

    ImGui::Columns(5, NULL, true);
    {
        if(DrawSL2Matrix(rotate1, ROTATE) && immediate_mode) {
            local_five.Modify(0, std::make_tuple(ROTATE, embedding, rotate1));
            changed = true;
        }
        if(ImGui::Button("Add##Rotate1")){
            SL4walk.Append(std::make_tuple(ROTATE, embedding, rotate1));
            changed = true;
        }
        ImGui::SameLine();
        if(ImGui::Button("0##Rotate1")) {
            rotate1=0;
            local_five.Modify(0, std::make_tuple(ROTATE, embedding, rotate1));
            changed = true;
        }
        ImGui::NextColumn();
    }
    {
        if(DrawSL2Matrix(shearL, SHEAR_L) && immediate_mode){
            local_five.Modify(1, std::make_tuple(SHEAR_L, embedding, shearL));
            changed = true;
        }
        if(ImGui::Button("Add##ShearL")){
            SL4walk.Append(std::make_tuple(SHEAR_L, embedding, shearL));
            changed = true;
        }
        ImGui::SameLine();
        if(ImGui::Button("0##ShearL1")) {
            shearL=0;
            local_five.Modify(1, std::make_tuple(SHEAR_L, embedding, shearL));
            changed = true;
        }
        ImGui::NextColumn();
    }
    {
        if(DrawSL2Matrix(dilate, DILATE) && immediate_mode)
        {
            changed = true;
            local_five.Modify(2, std::make_tuple(DILATE, embedding, dilate));
        }
        if(ImGui::Button("Add##Dilate"))	{
            SL4walk.Append(std::make_tuple(DILATE, embedding, dilate));
            changed = true;
        }
        ImGui::SameLine();
        if(ImGui::Button("0##dilate")) {
            changed = true;
            dilate=0;	           local_five.Modify(2, std::make_tuple(DILATE, embedding, dilate)); 	}
        ImGui::NextColumn();
    }
    {
        if(DrawSL2Matrix(shearU, SHEAR_U) && immediate_mode){
            changed = true;
            local_five.Modify(3, std::make_tuple(SHEAR_U, embedding, shearU));
        }
        if(ImGui::Button("Add##ShearU")){
            changed = true;
            SL4walk.Append(std::make_tuple(SHEAR_U, embedding, shearU));
        }
        ImGui::SameLine();
        if(ImGui::Button("0##shearU")) {
            changed = true;
            shearU=0;	           local_five.Modify(3, std::make_tuple(SHEAR_U, embedding, shearU));	 	}
        ImGui::NextColumn();
    }
    {
        if(DrawSL2Matrix(rotate2, ROTATE, 2) && immediate_mode){
            local_five.Modify(4, std::make_tuple(ROTATE, embedding, rotate2));
            changed = true;
        }
        if(ImGui::Button("Add##Rotate2")){
            SL4walk.Append(std::make_tuple(ROTATE, embedding, rotate2));
            changed = true;
        }
        ImGui::SameLine();
        if(ImGui::Button("0##rotate2")) {
            changed = true;
            rotate2=0;	  local_five.Modify(4, std::make_tuple(ROTATE, embedding, rotate2));        		}
        ImGui::NextColumn();
    }
    ImGui::Separator();
    if(immediate_mode) current_transform = local_five.Current();
    SL4walk.candidate_ = local_five.Current();
    for(const auto& n : local_five.SL4_stack_){
        DrawColumnMatrix4(n);
        ImGui::NextColumn();
    }
    return changed;
}

void MatrixWalkCandidate(MatrixWalk &SL4walk)
{
    ImGui::Columns(2);
    ImGui::Text("Total:");
    auto total = SL4walk.Candidate();
    if(ImGui::Button("Add##total"))	SL4walk.Append(total);
    ImGui::NextColumn();
    DrawColumnMatrix4(total);
    ImGui::Columns(1);
}

void SL4Control(MatrixWalk &SL4walk)
{
    if(SL4walk.SL4_stack_.empty()) ;
    else{
        ImGui::Columns(5, NULL, true);
        for(auto n : SL4walk.SL4_stack_){
            DrawColumnMatrix4(n);
            ImGui::NextColumn();
        }
    }
    ImGui::Columns(1);
}

auto Mat4ToEigen5(Mat4 in) -> Mat5f
{
    Eigen::Map<Eigen::Matrix4f> in_transformed(glm::value_ptr(in),4,4);
    Mat5f out = Mat5f::Identity();
    out.topLeftCorner<4,4>() = in_transformed;
    return out;
}

auto ChangeSampleSize(SampleSize& sample) -> bool
{
    bool changed = false;
    ImGui::Text("Sample Size Bounds:");
    static int max_bound = 50;
    ImGui::SliderInt("Bound limit", &max_bound, 0,200);

    ImGui::Text("[x-,x+]       [y-,y+]       [z-,z+]        [w-,w+]");
    if(sample.size()==10) {
        ImGui::SameLine();
        ImGui::Text("    [u-,u+]");
    }
    for(int i = 0, e = sample.size(); i<e; ++i) {
        std::string label = "##"+ std::to_string(i);
        int min = ( (i%2) == 0 ) ? -max_bound : sample[i-1];
        int max = ( (i%2) == 0 ) ? sample[i+1] : max_bound;
        if (ImGui::VSliderInt(label.c_str(), ImVec2(18, 60), &sample[i], min, max)) changed = true;
        if(i<e-1) ImGui::SameLine();
    }
    return changed;
}

