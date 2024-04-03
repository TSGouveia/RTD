using PimDeWitte.UnityMainThreadDispatcher;
using System.Collections;
using System.Collections.Generic;
using System.Threading;
using Unity.VisualScripting;
using UnityEngine;

public class SkillManager : MonoBehaviour
{
    private PortManager portManager;

    private void Start()
    {
        portManager = FindObjectOfType<PortManager>();
    }
    public void StartSkill(string skill)
    {
        UnityMainThreadDispatcher.Instance().Enqueue(DoSkill(skill));
    }
    IEnumerator DoSkill(string skill)
    {
        switch (skill)
        {
            case "C1SL1":
                StartCoroutine(C1SL1());
                break;
            case "SL1C2":
                StartCoroutine(SL1C2());
                break;
            case "C2C3":
                StartCoroutine(C2C3());
                break;
            case "C3SL2":
                StartCoroutine(C3SL2());
                break;
            case "SL2C4":
                StartCoroutine(SL2C4());
                break;
            case "mill":
                StartCoroutine(Mill());
                break;
            case "drill":
                StartCoroutine(Drill());
                break;
            /*case "C1C2":
                break;
            case "C3C4":
                break;*/
            default:
                Debug.LogError("Skill doens't exist");
                break;
        }
        yield return null;
    }
    IEnumerator C1SL1()
    {
        if (!portManager.GetPortValue('I', 7))
        {
            portManager.ChangePortValue('Q', 5, true);

            yield return new WaitUntil(() => portManager.GetPortValue('I', 5));

            Debug.Log("A");

            yield return new WaitForSeconds(2);

            Debug.Log("b");

            portManager.ChangePortValue('Q', 5, false);
        }
    }
    IEnumerator SL1C2()
    {
        portManager.ChangePortValue('Q', 6, true);

        StartCoroutine(Slider1());

        yield return new WaitUntil(() => !portManager.GetPortValue('I', 6));

        portManager.ChangePortValue('Q', 6, false);
    }
    IEnumerator C2C3()
    {
        portManager.ChangePortValue('Q', 6, true);
        portManager.ChangePortValue('Q', 8, true);

        yield return new WaitUntil(() => !portManager.GetPortValue('I', 8));

        portManager.ChangePortValue('Q', 6, false);
        portManager.ChangePortValue('Q', 8, false);
    }
    IEnumerator C3SL2()
    {
        portManager.ChangePortValue('Q', 8, true);

        yield return new WaitForSeconds(1);

        portManager.ChangePortValue('Q', 8, false);
    }
    IEnumerator SL2C4()
    {
        portManager.ChangePortValue('Q', 10, true);

        StartCoroutine(Slider2());

        yield return new WaitUntil(() => !portManager.GetPortValue('I', 9));

        portManager.ChangePortValue('Q', 10, false);
    }
    IEnumerator Slider1()
    {
        portManager.ChangePortValue('Q', 2, false);
        portManager.ChangePortValue('Q', 1, true);

        yield return new WaitUntil(() => GetSlider1Pos() == 1);

        portManager.ChangePortValue('Q', 1, false);
        portManager.ChangePortValue('Q', 2, true);

        yield return new WaitUntil(() => GetSlider1Pos() == 0);

        portManager.ChangePortValue('Q', 1, false);
        portManager.ChangePortValue('Q', 2, false);
    }
    IEnumerator Slider2()
    {
        portManager.ChangePortValue('Q', 4, false);
        portManager.ChangePortValue('Q', 3, true);

        yield return new WaitUntil(() => GetSlider2Pos() == 1);

        portManager.ChangePortValue('Q', 3, false);
        portManager.ChangePortValue('Q', 4, true);

        yield return new WaitUntil(() => GetSlider2Pos() == 0);

        portManager.ChangePortValue('Q', 3, false);
        portManager.ChangePortValue('Q', 4, false);
    }
    IEnumerator Mill()
    {
        portManager.ChangePortValue('Q', 7, true);

        yield return new WaitForSeconds(1);

        portManager.ChangePortValue('Q', 7, false);
    }
    IEnumerator Drill()
    {
        portManager.ChangePortValue('Q', 9, true);

        yield return new WaitForSeconds(1);

        portManager.ChangePortValue('Q', 9, false);
    }

    int GetSlider1Pos()
    {
        bool I1 = portManager.GetPortValue('I', 1);
        bool I2 = portManager.GetPortValue('I', 2);
        if (I2)
            return 0;  //if high level it's retracted
        else if (I1)
            return 1;      //if high level it's extended
        else return -1;  //if none are high level it's not on either ends
    }
    int GetSlider2Pos()
    {
        bool I3 = portManager.GetPortValue('I', 3);
        bool I4 = portManager.GetPortValue('I', 4);
        if (I4)
            return 0;  //if high level it's retracted
        else if (I3)
            return 1;      //if high level it's extended
        else return -1;  //if none are high level it's not on either ends
    }
}
