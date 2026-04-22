#include "PlayerAnimInstance.h"

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	// 现在的逻辑：不再主动“拉取”角色的状态。
	// 所有与状态机强绑定的变量（如 bIsMoving）将由具体的 State（如 PlayerWalkState）主动推送过来。
	// 这里可以保留一些纯表现层的计算（比如 IK 射线检测、脚部倾斜等），但不处理核心状态判断。
}
