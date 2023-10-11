#include "Camera.h"
#include"..\Input/Input.h"
#include "Game/MathHelper.h"
#include "Game/Manager/EnemyManager.h"
#include "imgui.h"

void Camera::SetLookAt(const DirectX::XMFLOAT3& eye, const DirectX::XMFLOAT3& focus, const DirectX::XMFLOAT3& up)
{
    //視点、注視点、上方向からビュー行列を作成
    Eye = DirectX::XMLoadFloat3(&eye);
    Focus = DirectX::XMLoadFloat3(&focus);
    Up = DirectX::XMLoadFloat3(&up);
    DirectX::XMMATRIX View = DirectX::XMMatrixLookAtLH(Eye, Focus, Up);

    // ビュー行列を逆行列化し、ワールド行列に戻す
    DirectX::XMMATRIX World = DirectX::XMMatrixInverse(nullptr, View);
    DirectX::XMFLOAT4X4 world;
    DirectX::XMStoreFloat4x4(&world, World);

    // カメラ方向を取り出す
    this->right.x = world._11;
    this->right.y = world._12;
    this->right.z = world._13;

    this->up.x = world._21;
    this->up.y = world._22;
    this->up.z = world._23;

    this->front.x = world._31;
    this->front.y = world._32;
    this->front.z = world._33;

}

void Camera::SetPerspectiveFov(Microsoft::WRL::ComPtr<ID3D11DeviceContext> dc)
{
    D3D11_VIEWPORT viewport{};
    UINT num_viewports{ 1 };
    dc->RSGetViewports(&num_viewports, &viewport);

    float aspect_ratio{ viewport.Width / viewport.Height };
    // 射影行列を生成
    P = { DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(30), aspect_ratio, 0.1f, 1000.0f) };

    // 視線行列を生成
    //DirectX::XMVECTOR eye{ DirectX::XMLoadFloat4(&camera_position) };
    V = { DirectX::XMMatrixLookAtLH(Eye, Focus, Up) };
}

void Camera::Update(float elapsedTime)
{
    GamePad& gamePad = Input::Instance().GetGamePad();

    if (gamePad.GetButtonDown() & GamePad::BTN_LEFT_SHOULDER)
    {
        InputLockOn();
    }


    if (is_lockOn) LockOnUpdate(elapsedTime);

    else NonLockOnUpdate(elapsedTime);



}

void Camera::DrawDebug()
{
    ImGui::Begin("Camera");
    ImGui::DragFloat3("eye", &eye.x);
    ImGui::DragFloat3("target", &target.x);
    ImGui::DragFloat("eye_y_offset", &eye_y_offset);
    ImGui::DragFloat("target_y_offset", &target_y_offset);
    ImGui::DragFloat("range", &range);
    ImGui::End();
}

void Camera::UpdateDebug()
{
}

void Camera::InputLockOn()
{
    // まだロックオンしてなかったらする
    if (!is_lockOn) {
        DirectX::XMFLOAT3 player_pos = CharacterManager::Instance().GetPlayer()->GetPosition();

        // 敵と自分との距離
        float player_to_enemy = FLT_MAX;

        // 全エネミーとの距離判定
        for (auto enemy : EnemyManager::Instance().GetEnemies())
        {
            DirectX::XMFLOAT3 enemy_pos = enemy->GetPosition();

            float length = Length(player_pos, enemy_pos);
            if (player_to_enemy > length)
            {
                is_lockOn = true;

                player_to_enemy = length;
                lock_on_enemy = enemy;
            }
        }
    }
    //ロックオンしてたら外す
    else
        is_lockOn = false;

}

void Camera::LockOnUpdate(float elapsedTime)
{
    DirectX::XMFLOAT3 enemy_pos = lock_on_enemy->GetPosition();
    DirectX::XMFLOAT3 player_pos = CharacterManager::Instance().GetPlayer()->GetPosition();

    float length = Length(player_pos, enemy_pos);

    // 敵からプレイヤーへのベクトル
    DirectX::XMFLOAT3 vec =
    {
         player_pos.x - enemy_pos.x ,
         player_pos.y - enemy_pos.y ,
         player_pos.z - enemy_pos.z ,
    };
    vec.x = vec.x / length;
    vec.y = vec.y / length;
    vec.z = vec.z / length;


    eye={
        player_pos.x + (vec.x * range),
        player_pos.y + (vec.y * range) + eye_y_offset,
        player_pos.z + (vec.z * range),
    };

    target = {
        enemy_pos.x,
        enemy_pos.y + target_y_offset,
        enemy_pos.z
    };
    //カメラの視点と注視点を設定
    SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));

}

void Camera::NonLockOnUpdate(float elapsedTime)
{
    GamePad& game_pad = Input::Instance().GetGamePad();

    /*----------- コントローラー処理 ------------*/
    float ax = game_pad.GetAxisRX();
    float ay = game_pad.GetAxisRY();

    float lx = game_pad.GetAxisLX();
    float ly = game_pad.GetAxisLY();

    // カメラの回転速度
    float speed = rollSpeed * elapsedTime;

    // スティックの入力値に合わせてX軸とY軸を回転
    angle.x -= ay * speed;
    angle.y += ax * speed;

#if 0
    /*------------- マウス処理 -------------*/
    Mouse& mouse = Input::Instance().GetMouse();

    float mouse_pos_x = static_cast<int> (mouse.GetPositionX() - mouse.GetOldPositionX());
    float mouse_pos_y = static_cast<int> (mouse.GetPositionY() - mouse.GetOldPositionY());

    // マウスでの感度の補正値、rollSpeedにかける。
    float speed_fit = 1.0f;

    // カメラの回転速度。
    speed = speed_fit * rollSpeed * elapsedTime;


    // スティックの入力値に合わせてX軸とY軸を回転。
    angle.x += mouse_pos_y * speed;
    angle.y += mouse_pos_x * speed;
# endif
    Lemur::Graphics::Graphics& graphics = Lemur::Graphics::Graphics::Instance();
#if 0
    SetCursorPos(1920 / 2, 1080 / 2);
    mouse.SetPositionX(1920 / 2);
    mouse.SetPositionY(1080 / 2);
#endif

    //target = CharacterManager::Instance().GetPlayer()->GetPosition();
    //target.y += target_y_offset;
    if(CharacterManager::Instance().GetPlayer()!=nullptr)target = CharacterManager::Instance().GetPlayer()->GetPosition();

    // カメラの回転値を回転行列に変換
    DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

    // 回転行列から前方向にベクトルを出す
    DirectX::XMVECTOR Front = Transform.r[2];
    DirectX::XMFLOAT3 front;
    DirectX::XMStoreFloat3(&front, Front);

    eye.x = target.x - (front.x * range);               // front=方向、長さを掛け算することで長さがわかる
    eye.y = target.y - (front.y * range) + eye_y_offset;// 目標地点からカメラの距離を引くことでカメラの位置が出る
    eye.z = target.z - (front.z * range);               // 解説８ページ
    // X軸のカメラ回転を制限
    angle.x = min(angle.x, maxAngleX);
    angle.x = max(angle.x, minAngleX);
    //模範解答↓
    if (angle.x < minAngleX)
    {
        angle.x = minAngleX;
    }
    if (angle.x > maxAngleX)
    {
        angle.x = maxAngleX;
    }

    // Y軸の回転値を-3.14~3.14に収まるようにする
    if (angle.y < -DirectX::XM_PI)
    {
        angle.y += DirectX::XM_2PI;
    }
    if (angle.y > -DirectX::XM_PI)
    {
        angle.y -= DirectX::XM_2PI;
    }

    //カメラの視点と注視点を設定
    SetLookAt(eye, target, DirectX::XMFLOAT3(0, 1, 0));

}
