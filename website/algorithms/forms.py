from django.contrib.auth.models import User 
from django import forms
from .models import Algorithm

class UserForm(forms.ModelForm):
    password = forms.CharField(widget = forms.PasswordInput)

    class Meta:
        model = User
        fields = ['username', 'email', 'password']

class LoginUserForm(forms.ModelForm):
    password = forms.CharField(widget = forms.PasswordInput)

    class Meta:
        model = User
        fields = ['username', 'password']


class CompareAlgorithms(forms.Form):
    algorithm1 = forms.ModelChoiceField(queryset = Algorithm.objects.all())
    algorithm2 = forms.ModelChoiceField(queryset = Algorithm.objects.all())
    function = forms.IntegerField()
    dimension = forms.IntegerField()